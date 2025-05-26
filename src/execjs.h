#ifndef FAKER_EXECJS_H
#define FAKER_EXECJS_H

#include <node.h>
#include <string.h>


using namespace v8;
using v8::FunctionCallbackInfo;


namespace faker {

void RunJS(const FunctionCallbackInfo<Value>& info);
void RunJSFile(const FunctionCallbackInfo<Value>& info);

}

#endif