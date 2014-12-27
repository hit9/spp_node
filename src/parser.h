#include <v8.h>
#include <node.h>
#include <spp.h>
#include "nan.h"

namespace spp {

using namespace v8;
using namespace node;

class Parser : public ObjectWrap {
public:
    Parser();
    ~Parser();

    static void Initialize(Handle<Object> exports);
    static NAN_METHOD(New);
    static NAN_METHOD(Feed);
    static NAN_METHOD(Get);

    Persistent<Array> handle;
private:

    spp_t* parser;
};

};
