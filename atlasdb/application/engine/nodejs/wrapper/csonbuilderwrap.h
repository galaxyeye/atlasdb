/*
 * consbuilderwrap.h
 *
 *  Created on: 2012-2-15
 *      Author: jy
 */

#ifndef CONSBUILDERWRAP_H_
#define CONSBUILDERWRAP_H_

#include <v8/v8.h>
#include <atlasdb/serialization/cson/csonbuilder.h>
#include <iostream>

#include "BSONObjwrap.h"

using namespace v8;
using namespace cson;

class cson_builder_wrapper {

public:
  BSONObj_wrapper BSONObj;

  cson_builder_wrapper() : builder_(NULL) { }
  ~cson_builder_wrapper() {
    if(builder_ != NULL) {
        delete builder_;
    }
  }

  v8::Handle<Value> init(const Arguments& args) {
    if(args.Length() != 1){
      return ThrowException(String::New("Expected 1 arguments"));
    }
    builder_ = new BSONObjBuilder(args[0]->Int32Value());
      return v8::True();
  }

  Handle<Value> append_string(const Arguments& args) {
    if(args.Length() != 1){
      return ThrowException(String::New("Expected 1 arguments"));
    }
    Local<v8::String> s = args[0]->ToString();
    char buffer[128];
    (*s)->WriteAscii(buffer);

    std::cout << "buffer: " << buffer << std::endl;

    builder_->appendString(buffer);
    return v8::True();
  }

  Handle<Value> append_number(const Arguments &args) {
    if(args.Length() != 1) {
      return ThrowException(String::New("Expected 1 arguments"));
    }

    if(args[0]->IsInt32()) {
      int ret = args[0]->Int32Value();
      builder_->appendNumber(ret);
    }
    else if(args[0]->IsNumber()) {
      double ret = args[0]->NumberValue();
      builder_->appendNumber(ret);
    }
    else {
      return ThrowException(String::New("Unexpected type"));
    }
    return v8::True();
  }

  Handle<Value> append_bool(const Arguments &args) {
    if(args.Length() != 1) {
      return ThrowException(String::New("Expected 1 arguments"));
    }
    if(args[0]->IsBoolean()) {
      bool ret = args[0]->BooleanValue();
      builder_->appendBool(ret);
      return v8::True();
    }
    else {
      return ThrowException(String::New("Unexpected type"));
    }
  }

  bool done() {
    BSONObj = builder_->done();
    //printf("%s\n", BSONObj.getStringField(0));
    return true;
  }

private:
  BSONObjBuilder *builder_;
};



#endif /* CONSBUILDERWRAP_H_ */
