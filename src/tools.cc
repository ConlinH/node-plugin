
#include "faker.h"

namespace faker {


void Print(const FunctionCallbackInfo<Value>& info) {
    // HandleScope handle_scope(info.GetIsolate());
    // Local<Object> temp = Object::New(info.GetIsolate());
    for (int i = 0; i < info.Length(); i++) {
        // temp->SetPrototype(info.GetIsolate()->GetCurrentContext(), info[i]);
        String::Utf8Value str(info.GetIsolate(), info[i]);
        printf("print: %s ", *str);
    }
    printf("\n");
    fflush(stdout);
}


void GetArg(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    Local<Context> ctx = isolate->GetCurrentContext();

    if (info.Length() == 2 && info[0]->IsObject() && info[1]->IsString()) {
        Local<Object> targert = info[0].As<Object>();
        Local<Value> val;
        if (targert->GetPrivate(ctx, v8::Private::ForApi(isolate, info[1].As<String>())).ToLocal(&val)) {
            info.GetReturnValue().Set(val);
            return;
        }
    }
    info.GetIsolate()->ThrowException(Exception::TypeError(V8String(isolate, "get arg failed")));
}

void SetArg(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    Local<Context> ctx = isolate->GetCurrentContext();

    if (info.Length() == 3 && info[0]->IsObject() && info[1]->IsString()) {
        Local<Object> targert = info[0].As<Object>();
        if (targert->SetPrivate(ctx, v8::Private::ForApi(isolate, info[1].As<String>()), info[2]).IsJust()) {
            info.GetReturnValue().Set(v8::True(isolate));
            return;
        }
    }
    info.GetIsolate()->ThrowException(Exception::TypeError(V8String(isolate, "set arg failed")));
}


}