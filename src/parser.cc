#include <v8.h>
#include <node.h>
#include <assert.h>
#include <string.h>
#include "parser.h"

using namespace spp;


static void handler(spp_t *parser, char *str, size_t len, int id) {
    Parser *p = reinterpret_cast<Parser*>(parser->priv);
    Local<Array> arr = NanNew<Array>(p->handle);
    arr->Set(id, NanNew<String>(str, len));
}

Parser::Parser()
{
    parser = spp_new();
    parser->priv = this;
    parser->handler = &handler;
}

Parser::~Parser() {
    spp_free(parser);
}


/**
 * Register prototypes and exports
 */
void Parser::Initialize(Handle<Object> exports) {
    NanScope();
    Local<FunctionTemplate> t = NanNew<FunctionTemplate>(New);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    NODE_SET_PROTOTYPE_METHOD(t, "feed", Feed);
    NODE_SET_PROTOTYPE_METHOD(t, "get", Get);
    exports->Set(NanNew<String>("Parser"), t->GetFunction());
}


/**
 * Public API - new Parser
 */
NAN_METHOD(Parser::New) {
    NanScope();
    Parser *parser = new Parser();
    parser->Wrap(args.This());
    NanReturnValue(args.This());
}


/**
 * Public API - Parser.prototype.feed
 */
NAN_METHOD(Parser::Feed) {
    NanScope();
    Parser *p = ObjectWrap::Unwrap<Parser>(args.This());
    v8::String::Utf8Value str(args[0]->ToString());
    char *buf = *str;
    int result = spp_feed(p->parser, buf, strlen(buf));
    NanReturnValue(NanNew<v8::Number>(result));
}


/**
 * Public API - Parser.prototype.get
 */
NAN_METHOD(Parser::Get) {
    NanScope();
    Local<Value> retv;
    Parser *p = ObjectWrap::Unwrap<Parser>(args.This());
    Local<Array> arr(NanNew<Array>());
    NanDisposePersistent(p->handle);
    NanAssignPersistent(p->handle, arr);
    int result = spp_parse(p->parser);

    if (result == SPP_OK) {
        retv = NanNew<Array>(p->handle);
        NanDisposePersistent(p->handle);
        NanReturnValue(retv);
    } else if (result == SPP_EBADFMT) {
        NanReturnUndefined();
    }
    NanReturnUndefined();
}
