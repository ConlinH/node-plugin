#include "faker.h"

namespace faker {

void NativeWrapperCallback(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();

    Local<Value> param[10];
    for (int i = 0; i < info.Length(); i++) {
        param[i] = info[i];
    }

    Local<Object> handler = info.Data().As<Object>();
    Local<Function> func = handler->Get(context, String::NewFromUtf8(isolate, "cb").ToLocalChecked()).ToLocalChecked().As<Function>();

    TryCatch tryCatch(isolate);
    MaybeLocal<Value> maybe_result = func->Call(context, info.This(), info.Length(), param);
    Local<Value> result;
    if (tryCatch.HasCaught()) {
        result = tryCatch.Exception();
        tryCatch.Reset();
        isolate->ThrowException(result);
        if (tryCatch.HasCaught()) {
            tryCatch.ReThrow();
        }
    }
    else {
        result = maybe_result.FromMaybe(Undefined(isolate).As<Value>());
        info.GetReturnValue().Set(result);
    }
}
    
void NativeWrapper(const FunctionCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();

    if (info.Length() > 0 && info[0]->IsObject()) {
        Local<Object> handler = info[0].As<Object>();
        if (handler->HasOwnProperty(context, String::NewFromUtf8(isolate, "cb").ToLocalChecked()).ToChecked()) {

            Local<FunctionTemplate> func_temp = FunctionTemplate::New(isolate, NativeWrapperCallback, handler);

            if (handler->HasOwnProperty(context, String::NewFromUtf8(isolate, "name").ToLocalChecked()).ToChecked()) {
                Local<String> name = handler->Get(context, String::NewFromUtf8(isolate, "name").ToLocalChecked()).ToLocalChecked().As<String>();
                func_temp->SetClassName(name);
            }

            if (handler->HasOwnProperty(context, String::NewFromUtf8(isolate, "length").ToLocalChecked()).ToChecked()) {
                Local<Number> length = handler->Get(context, String::NewFromUtf8(isolate, "length").ToLocalChecked()).ToLocalChecked().As<Number>();
                func_temp->SetLength(length->Value());
            }

            Local<Function>func = func_temp->GetFunction(context).ToLocalChecked();
            info.GetReturnValue().Set(func);
        }
    }
}

}