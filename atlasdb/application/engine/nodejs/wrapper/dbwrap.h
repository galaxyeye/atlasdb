/*
 * dbwrap.h
 *
 *  Created on: 2012-2-15
 *      Author: jy
 */

#ifndef DBWRAP_H_
#define DBWRAP_H_

#include <v8/v8.h>
#include <atlasdb/storage/basic_storehouse.h>
#include <atlasdb/storage/bdb_provider.h>
#include <atlasdb/serialization/cson/util/csonwriter.h>
#include "BSONObjwrap.h"

using namespace v8;
using namespace atlasdb::storage;
using namespace provider;

const int key_length = 129;

class key_wrapper {
public:

  key_wrapper() {
    length_ = key_length;
  }

  void *key() { return key_; }
  int &length() { return length_; }

private:
  char key_[key_length];
  int length_;
};

typedef BSONObj_wrapper value_wrapper;

typedef bdb_storage<>::key_type bdb_key_t;
typedef bdb_storage<>::data_type bdb_data_t;

class db_process {
public:
  key_wrapper key;
  value_wrapper value;

  db_process() : repository(NULL) { }

  ~db_process() {
    if(repository != NULL) {
      delete repository;
    }
  }

  bool insert_() {
    bdb_key_t k(key.key(), key.length());
    bdb_data_t v(value.data(), value.length());

    repository->put(k, v);
    return true;
  }

  bool update_() {
    bdb_key_t k(key.key(), key.length());
    bdb_data_t v(value.data(), value.length());
    repository->update(k, v);
    return true;
  }

  bool delete_() {
    bdb_key_t k(key.key(), key.length());
    repository->del(k);
    return true;
  }

  Handle<Value> insert(const Arguments &args) {
    if(args.Length() != 4) {
        return ThrowException(String::New("Expected 4 arguments"));
    }
    //if(!(args[0]->IsExternal() && args[2]->IsExternal())) {
    //    return Exception::TypeError(String::New("Expected external type"));
    //}

    void *key = External::Unwrap(args[0]);
    int key_len = args[1]->Int32Value();
    void *value = External::Unwrap(args[2]);

    int value_len = args[3]->Int32Value();

    bdb_key_t k(key, key_len);
    bdb_data_t v(value, value_len);

    if(!repository->put(k, v)) {
      std::cout << "ops " << std::endl;
      std::cout << "insert result, size: " << repository->get(k).second.size() << std::endl;
      return ThrowException(String::New("insert data failed"));
    }

    std::cout << "insert result, size: " << repository->get(k).second.size() << std::endl;
    return True();
  }

  Handle<Value> update(const Arguments &args) {
    if(args.Length() != 4) {
      return ThrowException(String::New("Expected 4 arguments"));
    }
    //if(!(args[0]->IsExternal() && args[2]->IsExternal())) {
    //  return Exception::TypeError(String::New("Expected external type"));
    //}

    void *key = External::Unwrap(args[0]);
    int key_len = args[1]->Int32Value();
    void *value = External::Unwrap(args[2]);
    int value_len = args[3]->Int32Value();

    bdb_key_t k(key, key_len);
    bdb_data_t v(value, value_len);

    repository->update(k, v);

    std::cout << "update result, size: " << repository->get(k).second.size() << std::endl;
    return True();
  }


  Handle<Value> del(const Arguments &args) {
    if(args.Length() != 2) {
      return ThrowException(String::New("Expected 2 arguments"));
    }
    //if(!(args[0]->IsExternal())) {
    //  return ThrowException(String::New("Expected external type"));
      //return Exception::TypeError(String::New("Expected external type"));
    //}

    void *key = External::Unwrap(args[0]);
    int key_len = args[1]->Int32Value();

    bdb_key_t k(key, key_len);
    repository->del(k);

    std::cout << "delete result, size: " << repository->get(k).second.size() << std::endl;
    return True();
  }

  Handle<Value> open_db(const Arguments &args) {
    if(args.Length() != 1) {
      return ThrowException(String::New("Expected 1 arguments"));
    }
    if(!args[0]->IsString()) {
      return Exception::TypeError(String::New("Expected string type"));
    }
    v8::Local<v8::String> s = args[0]->ToString();
    char buffer[128];
    (*s)->WriteAscii(buffer, 0, 128);

    repository = new basic_repository< bdb_storage<> >(buffer);

    return True();
  }

  bool close() {
    if(repository != NULL) {
      delete repository;
      repository = NULL;
    }
    return true;
  }

  Handle<Value> get_key(const Arguments &args) {
    HandleScope scope;
    return scope.Close(External::Wrap(key.key()));
  }

  int get_key_length() {
    return key.length();
  }

  Handle<Value> put_key(const Arguments &args) {
    if(args.Length() != 1) {
      return ThrowException(String::New("Expected 1 arguments"));
    }
    if(args[0]->IsInt32()) {
      int ret = args[0]->Int32Value();
      if(!cson::csonwriter::convert_int_buffer(ret, key.key(), key.length())) {
        return ThrowException(String::New("too huge key"));
      }
    }
    else if(args[0]->IsString()){
      Local<String> ret = args[0]->ToString();
      char buf[128];
      ret->WriteAscii(buf);
      if(!cson::csonwriter::convert_string_buffer(buf, key.key(), key.length())) {
        return ThrowException(String::New("too huge key"));
      }
    }
    //TODO ? other type
    else {
        return Exception::TypeError(String::New("Expected int, string type"));
    }

    return Undefined();
  }

private:
  basic_repository< bdb_storage<> > *repository;
};

#endif /* DBWRAP_H_ */
