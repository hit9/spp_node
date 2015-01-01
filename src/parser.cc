#include <v8.h>
#include <node.h>
#include <string.h>
#include <stdint.h>
#include "parser.h"

using namespace spp;


static void handler(spp_t *parser, char *data, size_t len, int id) {
    Parser *p = reinterpret_cast<Parser*>(parser->priv);
    Local<Array> arr = NanNew<Array>(p->handle);
    arr->Set(id, NanNew<String>(data, len));
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
    NODE_SET_PROTOTYPE_METHOD(t, "clear", Clear);
    exports->Set(NanNew<String>("Parser"), t->GetFunction());
}


/**
 * Public API - new Parser
 */
NAN_METHOD(Parser::New) {
    NanScope();
    if (args.Length() != 0) {
        NanThrowTypeError("Parser requires no arguments");
    } else {
        Parser *parser = new Parser();
        parser->Wrap(args.This());
        NanReturnValue(args.This());
    }
}


/**
 * Public API - Parser.prototype.feed
 */
NAN_METHOD(Parser::Feed) {
    NanScope();
    Parser *p = ObjectWrap::Unwrap<Parser>(args.This());

    if (args.Length() != 1) {
        NanThrowTypeError("Feed requires one argument");
    } else if (!args[0]->IsString() && !Buffer::HasInstance(args[0])) {
        NanThrowTypeError("Requires string/buffer");
    } else {
        String::Utf8Value s(args[0]->ToString());
        char *str = *s;
        int result = spp_feed(p->parser, str);
        switch(result) {
            case SPP_OK:
                // return the new size
                NanReturnValue(NanNew<Number>(p->parser->buf->size));
                break;
            case SPP_ENOMEM:
                NanThrowError("No memory");
                break;
        }
    }
}


/**
 * Public API - Parser.prototype.get
 */
NAN_METHOD(Parser::Get) {
    NanScope();

    if (args.Length() != 0) {
        NanThrowTypeError("Get requires no arguments");
    } else {
        Local<Value> retv;
        Parser *p = ObjectWrap::Unwrap<Parser>(args.This());
        Local<Array> arr(NanNew<Array>());
        NanDisposePersistent(p->handle);
        NanAssignPersistent(p->handle, arr);
        int result = spp_parse(p->parser);

        switch(result) {
            case SPP_OK:
                retv = NanNew<Array>(p->handle);
                NanDisposePersistent(p->handle);
                NanReturnValue(retv);
                break;
            case SPP_ENOMEM:
                NanThrowError("No memory");
                break;
            case SPP_EBADFMT:
                NanThrowError("Bad format");
                break;
            case SPP_EUNFINISH:
                NanReturnUndefined();
                break;
        }
    }
}


/**
 * Public API - Parser.prototype.clear
 */
NAN_METHOD(Parser::Clear) {
    NanScope();
    Parser *p = ObjectWrap::Unwrap<Parser>(args.This());
    spp_clear(p->parser);
}
