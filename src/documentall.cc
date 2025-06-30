#include "faker.h"  


namespace faker {

void DocumentAllCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();

    Local<Value> val;
    if (info.This()->GetPrivate(context, v8::Private::ForApi(isolate, V8String(info.GetIsolate(), "cb"))).ToLocal(&val))
    {
        if (!val->IsFunction()){
            info.GetReturnValue().SetNull();
            return;
        }
        Local<Value> param[10];
        for (int i = 0; i < info.Length(); i++)
        {
            param[i] = info[i];
        }
        Local<Function> call_all = val.As<Function>();
        Local<Value> result = call_all->Call(context, Null(isolate), info.Length(), param).ToLocalChecked();
        info.GetReturnValue().Set(result);
        return;
    }
    info.GetReturnValue().SetNull();
}

void IllegalConstructorCallback(const FunctionCallbackInfo<Value>& info) {
    info.GetIsolate()->ThrowException(v8::Exception::TypeError(V8String(info.GetIsolate(), "Illegal constructor")));
}

v8::Persistent<v8::FunctionTemplate> persistent_t;

void DocumentAll(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    HandleScope handle_scope(isolate);
    Local<Context> context = isolate->GetCurrentContext();
    EscapableHandleScope es(isolate);

    if (persistent_t.IsEmpty())
    {
        Local<FunctionTemplate> t = FunctionTemplate::New(isolate, IllegalConstructorCallback);
        t->SetClassName(V8String(isolate, "HTMLAllCollection"));
        t->PrototypeTemplate()->Set(Symbol::GetToStringTag(isolate), V8String(isolate, "HTMLAllCollection"), PropertyAttribute(v8::ReadOnly | v8::DontEnum));
        t->InstanceTemplate()->MarkAsUndetectable();
        t->InstanceTemplate()->SetCallAsFunctionHandler(DocumentAllCallback);
        persistent_t.Reset(isolate, t);
    }

    Local<Object> obj = persistent_t.Get(isolate)->InstanceTemplate()->NewInstance(context).ToLocalChecked();
    Local<Function> fn = persistent_t.Get(isolate)->GetFunction(context).ToLocalChecked();

    Local<Array> arr = Array::New(isolate, 2);
    arr->Set(context, 0, obj);
    arr->Set(context, 1, fn);
    info.GetReturnValue().Set(arr);
}

}