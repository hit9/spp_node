var util = require('util');
var spp = require('./build/Release/spp');
var parser = new spp.Parser();


parser.feed('2\nok\n4\nbody\n\n');
console.log(parser.get());
