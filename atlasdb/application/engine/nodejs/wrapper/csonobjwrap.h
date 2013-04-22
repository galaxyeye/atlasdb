/*
 * BSONObjwrap.h
 *
 *  Created on: 2012-2-15
 *      Author: jy
 */

#ifndef BSONObjWRAP_H_
#define BSONObjWRAP_H_

#include <v8/v8.h>
#include <iostream>
#include <exception>
#include <atlasdb/serialization/cson/BSONObj.h>

using namespace v8;

#define CHECK_ARGS \
  do { \
    if(args.Length() != 1) { \
      return ThrowException(String::New("Expected 1 arguments")); \
    } \
  } while(false)

class BSONObj_wrapper {

public:

  Handle<Value> get_string_field(const Arguments &args) {
    HandleScope scope;
    CHECK_ARGS;
    Local<String> s = args[0]->ToString();
    char buffer[128];
    (*s)->WriteAscii(buffer);
    const char *ret = BSONObj_.getStringField(buffer);
    return scope.Close(String::New(ret));
  }

  Handle<Value> get_int_field(const Arguments &args) {
    HandleScope scope;
    CHECK_ARGS;
    Local<String> s = args[0]->ToString();
    char buffer[128];
    (*s)->WriteAscii(buffer, 0, 128);
    int ret = BSONObj_.getIntField(buffer);
    return scope.Close(Int32::New(ret));
  }

  Handle<Value> get_bool_field(const Arguments &args) {
    HandleScope scope;
    CHECK_ARGS;
    Local<String> s = args[0]->ToString();
    char buffer[128];
    (*s)->WriteAscii(buffer, 0, 128);
    bool ret = BSONObj_.getBoolField(buffer);
    return scope.Close(Boolean::New(ret));
  }

  Handle<Value> get_date_field(const Arguments &args) {
    //TODO
    return Undefined();
  }

  Handle<Value> get_decimal_field(const Arguments &args) {
    //TODO
    return Undefined();
  }

  Handle<Value> get_long_field(const Arguments &args) {
    HandleScope scope;
    if(args.Length() != 1) {
      return ThrowException(String::New("Expected 1 arguments"));
    }
    Local<String> s = args[0]->ToString();
    char buffer[128];
    (*s)->WriteAscii(buffer);
    std::cout << "field " << buffer << ": ";
    long ret;
    //BSONObj.descript(128);
    try {
      ret = BSONObj_.getLongField(buffer);
    }
    catch(std::exception &e) {
      BSONObj_.descript(128);
      return ThrowException(String::New(e.what()));
    }
    std::cout << ret << std::endl;
    return scope.Close(Integer::New(ret));
  }

  long get_long_field_directly(const char *name) {
    std::cout << "field " << name << ": ";
    long ret;
    //BSONObj.descript(128);
    try {
      ret = BSONObj_.getLongField(name);
    }
    catch(std::exception &e) {
      BSONObj_.descript(128);
      throw;
    }
    std::cout << ret << std::endl;
    return ret;
  }

  Handle<Value> js_data(const Arguments &args) {
    HandleScope scope;
    void *ret = BSONObj_.objdata();
    return scope.Close(External::Wrap(ret));
  }

  void *data() { return BSONObj_.objdata(); }

  int length() { return BSONObj_.objsize(); }

  BSONObj_wrapper() {}
  BSONObj_wrapper(char *data) : BSONObj_(data) {  }

  void init(char *data) {
    cson::BSONObj tmp(data);
    BSONObj_ = tmp;
  }

  BSONObj_wrapper& operator= (const cson::BSONObj &obj) {
    BSONObj_ = obj;
    BSONObj_.descript(64);
    return *this;
  }

  void init(const cson::BSONObj &obj) {
    BSONObj_ = obj;
  }

private:
  cson::BSONObj BSONObj_;
};


#endif /* BSONObjWRAP_H_ */
