#include "faker.h"


namespace faker {

// Extracts a C string from a V8 Utf8Value.
const char* ToCString(const String::Utf8Value& value) {
    return *value ? *value : "<string conversion failed>";
}

void ReportException(Isolate* isolate, TryCatch* try_catch) {
    HandleScope handle_scope(isolate);
    String::Utf8Value exception(isolate, try_catch->Exception());
    const char* exception_string = ToCString(exception);
    Local<Message> message = try_catch->Message();
    if (message.IsEmpty()) {
        // V8 didn't provide any extra information about this error; just
        // print the exception.
        fprintf(stderr, "%s\n", exception_string);
    }
    else {
        // Print (filename):(line number): (message).
        String::Utf8Value filename(isolate,
            message->GetScriptOrigin().ResourceName());
        Local<Context> context(isolate->GetCurrentContext());
        const char* filename_string = ToCString(filename);
        int linenum = message->GetLineNumber(context).FromJust();
        fprintf(stderr, "%s:%i: %s\n", filename_string, linenum, exception_string);
        // Print line of source code.
        String::Utf8Value sourceline(
            isolate, message->GetSourceLine(context).ToLocalChecked());
        const char* sourceline_string = ToCString(sourceline);
        fprintf(stderr, "%s\n", sourceline_string);
        // Print wavy underline (GetUnderline is deprecated).
        int start = message->GetStartColumn(context).FromJust();
        for (int i = 0; i < start; i++) {
            fprintf(stderr, " ");
        }
        int end = message->GetEndColumn(context).FromJust();
        for (int i = start; i < end; i++) {
            fprintf(stderr, "^");
        }
        fprintf(stderr, "\n");
        Local<Value> stack_trace_string;
        if (try_catch->StackTrace(context).ToLocal(&stack_trace_string) &&
            stack_trace_string->IsString() &&
            stack_trace_string.As<String>()->Length() > 0) {
            String::Utf8Value stack_trace(isolate, stack_trace_string);
            const char* err = ToCString(stack_trace);
            fprintf(stderr, "%s\n", err);
        }
    }
}

// Executes a string within the current v8 context.
bool ExecuteString(Isolate* isolate, Local<String> source,
    Local<Value> name, bool print_result,
    bool report_exceptions) {
    HandleScope handle_scope(isolate);
    TryCatch try_catch(isolate);
    ScriptOrigin origin(isolate, name);
    Local<Context> context(isolate->GetCurrentContext());
    Local<Script> script;
    if (!Script::Compile(context, source, &origin).ToLocal(&script)) {
        // Print errors that happened during compilation.
        if (report_exceptions)
            ReportException(isolate, &try_catch);
        return false;
    }
    else {
        Local<Value> result;
        if (!script->Run(context).ToLocal(&result)) {
            assert(try_catch.HasCaught());
            // Print errors that happened during execution.
            if (report_exceptions)
                ReportException(isolate, &try_catch);
            return false;
        }
        else {
            assert(!try_catch.HasCaught());
            if (print_result && !result->IsUndefined()) {
                // If all went well and the result wasn't undefined then print
                // the returned value.
                String::Utf8Value str(isolate, result);
                const char* cstr = ToCString(str);
                printf("%s\n", cstr);
            }
            return true;
        }
    }
}

// Reads a file into a v8 string.
MaybeLocal<String> ReadFile(Isolate* isolate, const char* name) {
    FILE* file = fopen(name, "rb");
    if (file == NULL) return MaybeLocal<String>();

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    char* chars = new char[size + 1];
    chars[size] = '\0';
    for (size_t i = 0; i < size;) {
        i += fread(&chars[i], 1, size - i, file);
        if (ferror(file)) {
            fclose(file);
            return MaybeLocal<String>();
        }
    }
    fclose(file);
    MaybeLocal<String> result = String::NewFromUtf8(isolate, chars, NewStringType::kNormal, static_cast<int>(size));
    delete[] chars;
    return result;
}

void RunJS(const FunctionCallbackInfo<Value>& info) {
    if (info.Length() != 2) {
        info.GetIsolate()->ThrowError("Bad parameters");
        return;
    }
    HandleScope handle_scope(info.GetIsolate());
    Local<String> source = info[0].As<String>();
    if (!ExecuteString(info.GetIsolate(), source, info[1], false, true)) {
        info.GetIsolate()->ThrowError("Error executing js");
        return;
    }
}

void RunJSFile(const FunctionCallbackInfo<Value>& info) {
    if (info.Length() != 2) {
        info.GetIsolate()->ThrowError("Bad parameters");
        return;
    }
    HandleScope handle_scope(info.GetIsolate());
    String::Utf8Value file(info.GetIsolate(), info[0]);
    if (*file == NULL) {
        info.GetIsolate()->ThrowError("Error loading file");
        return;
    }
    Local<String> source;
    if (!ReadFile(info.GetIsolate(), *file).ToLocal(&source)) {
        info.GetIsolate()->ThrowError("Error loading file");
        return;
    }
    if (!ExecuteString(info.GetIsolate(), source, info[1], false, true)) {
        info.GetIsolate()->ThrowError("Error executing file");
        return;
    }
}

}