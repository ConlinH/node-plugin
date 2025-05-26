#include "faker.h"
#include "tools.h"
#include "execjs.h"
#include "native.h"
#include "documentall.h"  

using namespace v8;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::Value;
using v8::Context;
using v8::FunctionTemplate;
using v8::ScriptOrigin;
using v8::Script;
using v8::PropertyAttribute;

namespace faker {


Local<Object> Register(Local<Object> exports, Isolate* isolate, Local<Context> ctx) {
    v8::Local<v8::FunctionTemplate> interface_template_Faker = FunctionTemplate::New(isolate);
    Local<ObjectTemplate> prototype_template_Faker = interface_template_Faker->PrototypeTemplate();
    interface_template_Faker->SetLength(0);
    interface_template_Faker->SetClassName(V8String(isolate, "faker"));

    prototype_template_Faker->Set(Symbol::GetToStringTag(isolate), V8String(isolate, "faker"), PropertyAttribute(v8::ReadOnly | v8::DontEnum));
    
    // dall
    prototype_template_Faker->Set(isolate, "new_document_all", FunctionTemplate::New(isolate, DocumentAll));

    // native
    prototype_template_Faker->Set(isolate, "native", FunctionTemplate::New(isolate, NativeWrapper));

    // execjs
    prototype_template_Faker->Set(isolate, "run_js", FunctionTemplate::New(isolate, RunJS)); 
    prototype_template_Faker->Set(isolate, "run_js_file", FunctionTemplate::New(isolate, RunJSFile));
    prototype_template_Faker->Set(isolate, "v8_version", V8String(isolate, v8::V8::GetVersion()));
    
    // tools
    prototype_template_Faker->Set(isolate, "print", FunctionTemplate::New(isolate, Print));
    prototype_template_Faker->Set(isolate, "get_arg", FunctionTemplate::New(isolate, GetArg));
    prototype_template_Faker->Set(isolate, "set_arg", FunctionTemplate::New(isolate, SetArg));

    Local<Object> faker_obj = interface_template_Faker->InstanceTemplate()->NewInstance(ctx).ToLocalChecked();
    return faker_obj;
}


void Init(Local<Object> exports) {
    Isolate* isolate = exports->GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> faker = Register(exports, isolate, context);
    exports->SetPrototype(context, faker);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}