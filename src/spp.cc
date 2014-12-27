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
