var util = require('util');
var assert = require('assert');
var spp = require('./build/Release/spp');
var parser = new spp.Parser();


var n = 10000;

for (var i = 0; i < n; i++) {
  var istr = i.toString();
  var ilen = Buffer.byteLength(istr);
  parser.feed(util.format('2\nok\n%d\n%s\n\n', ilen, istr));
}

var startAt = new Date();
for (var i = 0; i < n; i++) {
  assert.deepEqual(parser.get(), ['ok', i]);
}
var endAt = new Date();
var ops = n * 1000 / (endAt - startAt);
console.log(util.format('%s ops',  ops));
