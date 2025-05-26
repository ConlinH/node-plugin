#include "faker.h"  


namespace faker {

void DocumentAllCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();

    if (info.Data().IsEmpty()) {
        info.GetReturnValue().SetNull();
    }
    else {
        Local<Value> param[10];
        for (int i = 0; i < info.Length(); i++) {
            param[i] = info[i];
        }
        Local<Function> call_all = info.Data().As<Function>();
        Local<Value> result = call_all->Call(context, Null(isolate), info.Length(), param).ToLocalChecked();
        info.GetReturnValue().Set(result);
    }

}

void IllegalConstructorCallback(const FunctionCallbackInfo<Value>& info) {
    info.GetIsolate()->ThrowException(v8::Exception::TypeError(V8String(info.GetIsolate(), "Illegal constructor")));
}

void DocumentAll(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();

    Local<FunctionTemplate> t = FunctionTemplate::New(isolate, IllegalConstructorCallback);
    t->InstanceTemplate()->MarkAsUndetectable();
    t->SetClassName(V8String(isolate, "HTMLAllCollection"));
    t->PrototypeTemplate()->Set(Symbol::GetToStringTag(isolate), V8String(isolate, "HTMLAllCollection"), PropertyAttribute(v8::ReadOnly | v8::DontEnum));

    if (info.Length() > 0 && info[0]->IsFunction()) {
        Local<Function> call_all = info[0].As<Function>();
        t->InstanceTemplate()->SetCallAsFunctionHandler(DocumentAllCallback, call_all);
    }
    else {
        t->InstanceTemplate()->SetCallAsFunctionHandler(DocumentAllCallback);
    }

    Local<Object> obj = t->InstanceTemplate()->NewInstance(context).ToLocalChecked();
    Local<Function> fn = t->GetFunction(context).ToLocalChecked();

    Local<Array> arr = Array::New(isolate, 2);
    arr->Set(context, 0, obj);
    arr->Set(context, 1, fn);
    info.GetReturnValue().Set(arr);
}

}