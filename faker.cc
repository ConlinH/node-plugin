
#include <node.h>
#include <string.h>


namespace faker {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::Value;
using v8::Context;
using v8::FunctionTemplate;

static void ReturnThis(const FunctionCallbackInfo<Value>& args) {
  args.GetReturnValue().Set(args.This());
}


void DocumentAll(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  Local<FunctionTemplate> desc = FunctionTemplate::New(isolate);
  desc->InstanceTemplate()->MarkAsUndetectable();  // undetectable
  desc->InstanceTemplate()->SetCallAsFunctionHandler(ReturnThis);  // callable

  Local<Object> obj = desc->GetFunction(context).ToLocalChecked()
                            ->NewInstance(context).ToLocalChecked();
  printf("Create <document.all> Object From faker.node\n");
  args.GetReturnValue().Set(obj);
}


void Initialize(Local<Object> exports) {
  NODE_SET_METHOD(exports, "DocumentAll", DocumentAll);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

}
