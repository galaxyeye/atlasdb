/*
 * querywrap.h
 *
 *  Created on: 2012-2-15
 *      Author: jy
 */

#ifndef QUERYWRAP_H_
#define QUERYWRAP_H_

#include <v8/v8.h>
#include <atlasdb/storage/basic_storehouse.h>
#include <atlasdb/storage/bdb_provider.h>

#include "BSONObjwrap.h"

using namespace v8;
using namespace atlasdb::storage;
using namespace provider;

class query_result {

public:
  query_result() : repository(NULL), first_flag(true) {};
  ~query_result() {
    if(repository != NULL) {
      delete(repository);
    }
  }

  void init(const string &dbname) {
    //std::cout << "dbname: " << dbname << std::endl;
    repository = new basic_repository< bdb_storage<> >(dbname);
    begin = repository->begin();
    end = repository->end();
  }

  bool get_next() {
    if(!first_flag) {
        begin++;
    }
    else {
        first_flag = false;
    }
    if(begin != end) {
      csonwrapper.init((char *)((*begin).second.address()));
      //begin++;
      return true;
    }
    else {
      return false;
    }
  }

  BSONObj_wrapper csonwrapper;

private:

  basic_repository< bdb_storage<> > *repository;
  bool first_flag;

  basic_repository< bdb_storage<> >::iterator begin;
  basic_repository< bdb_storage<> >::iterator end;
};


class query_process {

public:
  Handle<Value> load_all(const Arguments & args) {
    if(args.Length() != 1){
      return ThrowException(v8::String::New("Expected 1 arguments"));
    }
    v8::Local<v8::String> s = args[0]->ToString();
    char buffer[128];
    (*s)->WriteAscii(buffer, 0, 128);

    result.init(buffer);

    return v8::True();
  }

  bool load_all_directly(const string &name) {
    result.init(name);
    return true;
  }

  query_result result;

private:


};



#endif /* QUERYWRAP_H_ */
