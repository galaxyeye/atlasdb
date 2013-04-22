/*
* manipulator.h
*
*  Created on: Jun 10, 2012
*      Author: vincent
*/

#ifndef QUERY_MANIPULATOR_H_
#define QUERY_MANIPULATOR_H_

#include <string>
#include <vector>

#include <atlasdb/query/storage.h>

namespace atlasdb {
  namespace query {

    class stomanip {
    public:

      static void count(const string& table) {
        repository storage;
        return storage.count();
      }

      static void load();

      static void load_by_pkey(const string& table, const string& pkey);

      static void load_by_key(const string& table, const string& index, const string& pkey);

      static void load_field_by_pkey(const string& table, const string& pkey);

      static void load_field_by_key(const string& table, const string& index, const string& pkey);

      static void update_by_pkey(const string& table, const string& pkey);
      static void update_by_key(const string& table, const string& index, const string& pkey);
      static void update_field_by_pkey(const string& table, const string& pkey);
      static void update_field_by_key(const string& table, const string& index, const string& pkey);

      static void insert_key_value(const string& table, const string& pkey);

      static void remove_by_pkey();
      static void remove_by_key();
    };

  } // query
} // atlasdb

#endif // QUERY_MANIPULATOR_H_
