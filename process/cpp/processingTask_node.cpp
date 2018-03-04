//
//  processingTask.cpp
//
//  Created by Farshad on 3/2/18.
//
//

#include <stdio.h>
#include <node.h>
#include <v8.h>
#include <uv.h>
#include "processingTask.h"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace v8;

Tweet unpack_tweet(Isolate * , const Handle<Object> tweet_obj);
SubFrame unpack_subFrame(Isolate * , const Handle<Object> );
void pack_process_result(v8::Isolate* isolate, v8::Local<v8::Object> & target, ProcessResult & result);

struct Work {
    uv_work_t  request;
    Persistent<Function> callback;
    Tweet tweet;
    ProcessResult results;
};

// called by libuv worker in separate thread
static void work_async(uv_work_t *req)
{
    Work *work = static_cast<Work *>(req->data);
    // this is the worker thread, lets build up the results
    // allocated results from the heap because we'll need
    // to access in the event loop later to send back
  //  std::transform(work->tweet.subFrame.begin(), work->tweet.subFrame.end(), work->results.begin(), calc_process_stats);
    work->results=calc_process_stats(work->tweet);
     // work->results=first_subFrame_process(work->tweet);
    // that wasn't really that long of an operation, so lets pretend it took longer...
    std::this_thread::sleep_for(chrono::seconds(2));
}

// called by libuv in event loop when async function completes
static void work_async_complete(uv_work_t *req,int status)
{
    Isolate * isolate = Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate);
    Local<Array> result_list = Array::New(isolate);
    Work *work = static_cast<Work *>(req->data);
        Local<Object> result = Object::New(isolate);
        pack_process_result(isolate, result, work->results);
         std::cout<<"work->results="<<work<<endl;
        result_list->Set(0, result);
    // set up return arguments
    Handle<Value> argv[] = { Null(isolate) , result_list };
    Local<Function>::New(isolate, work->callback)->Call(isolate->GetCurrentContext()->Global(), 2, argv);
    // Free up the persistent function callback
    work->callback.Reset();
    delete work;
}

void native_calculate_results_async(const v8::FunctionCallbackInfo<v8::Value>&args) {
     Work *work =new Work();
     Isolate* isolate = args.GetIsolate();
     Tweet tweet = unpack_tweet(isolate, Local<Object>::Cast(args[0]));


     work->tweet = tweet;
     work->request.data=work;
     Local<Function> callback = Local<Function>::Cast(args[1]);
     work->callback.Reset(isolate, callback);
     uv_queue_work(uv_default_loop(),&work->request,work_async,work_async_complete);
     args.GetReturnValue().Set(Undefined(isolate));
}

Tweet unpack_tweet(Isolate * isolate, const Handle<Object> tweet_obj) {
    Tweet tweet;
    Handle<Value> myid = tweet_obj->Get(String::NewFromUtf8(isolate,"id"));
    Handle<Value> mycategory = tweet_obj->Get(String::NewFromUtf8(isolate,"category"));
    tweet.id = myid->NumberValue();
    tweet.category = mycategory->NumberValue();

    Handle<Array> array = Handle<Array>::Cast(tweet_obj->Get(String::NewFromUtf8(isolate,"subFrame")));
    int subFrame_count = array->Length();
    for ( int i = 0; i < subFrame_count; i++ ) {
        SubFrame sub = unpack_subFrame(isolate, Handle<Object>::Cast(array->Get(i)));
        tweet.subFrame.push_back(sub);
    }
    return tweet;
}


SubFrame unpack_subFrame(Isolate * isolate, const Handle<Object> subFrame_obj) {
  SubFrame s;
  Handle<Value> date_Value =
               subFrame_obj->Get(String::NewFromUtf8(isolate, "date"));
  Handle<Value> time_Value =
              subFrame_obj->Get(String::NewFromUtf8(isolate, "time"));
  Handle<Value> value_Value =
              subFrame_obj->Get(String::NewFromUtf8(isolate, "value"));
  v8::String::Utf8Value utfValue(date_Value);
  v8::String::Utf8Value utfTime(time_Value);
  s.date = std::string(*utfValue);
  s.time =  std::string(*utfTime);
  s.value = value_Value->NumberValue();
  return s;
}

void pack_process_result(v8::Isolate* isolate, v8::Local<v8::Object> & target, ProcessResult & result){
    target->Set(String::NewFromUtf8(isolate, "size"), Number::New(isolate, result.size));
    target->Set(String::NewFromUtf8(isolate, "status"), Number::New(isolate, result.status));
}



void init(Handle <Object> exports, Handle<Object> module) {
    NODE_SET_METHOD(exports, "calculate_results_async", native_calculate_results_async);
}

NODE_MODULE(processingTask, init)