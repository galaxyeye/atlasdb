/*
 * db_implementation.h
 *
 *  Created on: Apr 12, 2013
 *      Author: vincent
 */

#ifndef DB_IMPLEMENTATION_H_
#define DB_IMPLEMENTATION_H_

#include <string>
#include <vector>
#include <functional>
#include <memory>

#include <atlasdb/db/context.h>
#include <atlasdb/query/engine.h>
#include <boost/property_tree/ptree.hpp>

namespace atlasdb {
  namespace db {

    using std::string;
    using boost::property_tree::ptree;

    namespace detail {

      class db {

        db() {}

      public:

        void count(const string& table);

        void load();
        void load_by_pkey(const string& table, const string& pkey);
        void load_by_key(const string& table, const string& index, const string& pkey);
        void load_field_by_pkey(const string& table, const string& pkey);
        void load_field_by_key(const string& table, const string& index, const string& pkey);

        void find(const ptree& q);

        void update(const ptree& q);
        void update_by_pkey(const string& table, const string& pkey);
        void update_by_key(const string& table, const string& index, const string& pkey);
        void update_field_by_pkey(const string& table, const string& pkey);
        void update_field_by_key(const string& table, const string& index, const string& pkey);

        void insert(const ptree& q);
        void insert_key_value(const string& table, const string& pkey);

        void remove(const ptree& q);
        void remove_by_pkey();
        void remove_by_key();

        int insert(const ptree& q);

      private:

        engine_ptr _engine;
      };

    } // detail
  } // db
} // atlasdb

#endif /* DB_IMPLEMENTATION_H_ */
