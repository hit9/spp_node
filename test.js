/**
 * test big buffer
 */

var util = require('util');
var spp = require('./build/Release/spp');
var parser = new spp.Parser();

function randomString(length) {
  var chars = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXTZabcdefghiklmnopqrstuvwxyz'
              .split('');

  if (! length) {
    length = Math.floor(Math.random() * chars.length);
  }

  var str = '';
  for (var i = 0; i < length; i++) {
    str += chars[Math.floor(Math.random() * chars.length)];
  }
  return str;
}


var s = randomString(65539);
parser.feed(new Buffer(util.format('%d\n3%s\n\n', s.length, s)));
parser.get();
