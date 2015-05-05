var util = require('util');
var assert = require('assert');
var spp = require('./build/Release/spp');
var parser = new spp.Parser();

var n = 100000;

/********************  SPP  **************************/

var startAt = new Date();

for (var i = 0; i < n; i++) {
  var istr = i.toString();
  var ilen = Buffer.byteLength(istr);
  parser.feed(util.format('2\nok\n%d\n%s\n\n', ilen, istr));
  assert.deepEqual(parser.get(), ['ok', i]);
}

var endAt = new Date();
var ops = (n * 1000) / (endAt - startAt);
console.log(util.format('spp parser: %d in %ss => %d ops', n, (endAt - startAt)/1000, parseInt(ops)));


/******************** Nodejs Implementation *************************/

function NodejsParser(){}

NodejsParser.prototype.feed = function(buf) {
  if (typeof this.buf === 'undefined') {
    this.buf = new Buffer(buf);
  } else {
    this.buf = Buffer.concat([this.buf, new Buffer(buf)]);
  }
};

NodejsParser.prototype.get = function() {
  var len = this.buf.length;
  var ptr = 0;
  var ch = 0;
  var chunk = [];

  while (len > 0) {
    ch = [].indexOf.apply(this.buf, [10, ptr]);

    if (ch < 0) {
      break;
    }

    ch += 1;

    var dis = ch - ptr;

    if (dis === 1 || (dis === 2 && this.buf[ptr] === 13)) {
      this.buf = this.buf.slice(ch);
      return chunk;  // OK
    }

    var sz = parseInt(this.buf.slice(ptr, ch), 10);

    len -= dis + sz;
    ptr += dis + sz;

    if (len < 0) break;

    if (len >= 1 && this.buf[ptr] === 10) {
      len -= 1;
      ptr += 1;
    } else if (len >= 2 && this.buf[ptr] === 13 && this.buf[ptr + 1] === 10) {
      len -= 2;
      ptr += 2;
    } else {
      break;
    }
    chunk.push(this.buf.slice(ch, ch + sz).toString());
  }
};


var nodejsParser = new NodejsParser();

var startAt = new Date();

for (var i = 0; i < n; i++) {
  var istr = i.toString();
  var ilen = Buffer.byteLength(istr);
  nodejsParser.feed(util.format('2\nok\n%d\n%s\n\n', ilen, istr));
  assert.deepEqual(nodejsParser.get(), ['ok', i]);
}

var endAt = new Date();
var ops = (n * 1000) / (endAt - startAt);
console.log(util.format('nodejs parser: %d in %ss => %d ops', n, (endAt - startAt)/1000, parseInt(ops)));
