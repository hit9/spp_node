/**
 * Example using with Nodejs
 *
 *   var ssp = require('ssp');
 *   var parser = new ssp.Parser();
 *
 *   parser.feed('4\ndata\n\n');  // or multiple
 *
 *   var reply;
 *   while ((reploy = parser.get()) !== undefined) {
 *     ...
 *   }
 * 
 */

#include <v8.h>
#include <node.h>
#include "parser.h"

using namespace v8;

extern "C" {
    static void init (Handle<Object> exports) {
        NanScope();
        spp::Parser::Initialize(exports);
    }
    NODE_MODULE(spp, init);
}
