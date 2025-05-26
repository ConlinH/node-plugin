#ifndef FAKER_FAKER_H
#define FAKER_FAKER_H

#include <node.h>
#include <string.h>


using namespace v8;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::Array;
using v8::Value;
using v8::Context;
using v8::FunctionTemplate;
using v8::ScriptOrigin;
using v8::Script;
using v8::PropertyAttribute;


namespace faker {

#define V8String(isolate, str) String::NewFromUtf8(isolate, str).ToLocalChecked()

// faker
Local<Object> Register(Local<Object> exports, Isolate* isolate, Local<Context> ctx);

}

#endif