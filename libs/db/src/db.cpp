/*
 * db.cpp
 *
 *  Created on: Apr 21, 2013
 *      Author: vincent
 */

#include <atlasdb/db/db.h>
#include <atlasdb/query/stomanip.h>

namespace atlasdb {
  namespace db {

    // api for es
    /*
     * Indicate whether the given table exists
     * */
    bool db::exists(const string& table) {
      return true;
    }

    /*
     * Create an empty table
     * TODO : what about it's indexes?
     * */
    void db::create(const string& table) {}

    /*
     * Drop an table, including all indexes
     * */
    void db::drop(const string& table) {}

    /*
     * Make an table empty
     * */
    void db::truncate(const string& table) {}

    void db::count(const string& table) {}

    void db::load(const string& table) {

    }

    void db::load_by_pkey(const string& table, const string& pkey) {}
    void db::load_by_key(const string& table, const string& index, const string& pkey) {}
    void db::load_field_by_pkey(const string& table, const string& pkey) {}
    void db::load_field_by_key(const string& table, const string& index, const string& pkey) {}

    void db::find(const ptree& q) {}

    void db::update(const ptree& q) {}
    void db::update_by_pkey(const string& table, const string& pkey) {}
    void db::update_by_key(const string& table, const string& index, const string& pkey) {}
    void db::update_field_by_pkey(const string& table, const string& pkey) {}
    void db::update_field_by_key(const string& table, const string& index, const string& pkey) {}

    void db::insert(const ptree& q) {

    }

    void db::insert_key_value(const string& table, const string& pkey) {

    }

    void db::remove(const ptree& q) {}
    void db::remove_by_pkey() {}
    void db::remove_by_key() {}

  } // db
} // atlasdb
