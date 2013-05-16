/*
* manipulator.h
*
*  Created on: Jun 10, 2012
*      Author: vincent
*/

#ifndef QUERY_MANIPULATOR_H_
#define QUERY_MANIPULATOR_H_

#include <string>
#include <sstream>

#include <atlasdb/query/storage.h>

namespace atlasdb {
  namespace query {

    using std::string;
    using std::stringstream;
    using std::stringbuf;

    class stomanip {
    public:

      static size_t count(const string& table) {
        repository storage(table);
        return storage.count();
      }

      static stringbuf load(const string& table, size_t start = 0, size_t limit = 100) {
        stringstream ss;

        repository storage(table);

        size_t scan_count = 0;
        size_t load_count = 0;

        ss << 0;
        for (auto it = storage.begin(); it != storage.end(); ++it) {
          if (scan_count < start) ++scan_count;
          else if (load_count <= limit) {
            ss.write(it->second.address(), it->second.size());
            ++load_count;
          }
        }

        // ss.rdbuf()->
        return ss.rdbuf();
      }

      static stringbuf load_by_pkey(const string& table, const string& pkey);

      static stringbuf load_by_key(const string& table, const string& index, const string& pkey);

      static stringbuf load_field_by_pkey(const string& table, const string& pkey);

      static stringbuf load_field_by_key(const string& table, const string& index, const string& pkey);

      static stringbuf update_by_pkey(const string& table, const string& pkey);

      static stringbuf update_by_key(const string& table, const string& index, const string& pkey);

      static stringbuf update_field_by_pkey(const string& table, const string& pkey);

      static stringbuf update_field_by_key(const string& table, const string& index, const string& pkey);

      static stringbuf insert_key_value(const string& table, const string& pkey);

      static stringbuf remove_by_pkey();

      static stringbuf remove_by_key();
    };

  } // query
} // atlasdb

#endif // QUERY_MANIPULATOR_H_
