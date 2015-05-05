var assert = require('assert');
var spp = require('./index');
var parser = new spp.Parser();

parser.feed('2\nok\n\n');
assert.deepEqual(parser.get(), ['ok']);

parser.feed('2\nok\n');
parser.feed('3\nval\n');
parser.feed('\n');
assert.deepEqual(parser.get(), ['ok', 'val']);
