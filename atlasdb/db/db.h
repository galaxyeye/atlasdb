/*
 * db.h
 *
 *  Created on: Jun 10, 2012
 *      Author: vincent
 */

#ifndef atlasdb_DB_DB_H_
#define atlasdb_DB_DB_H_

#include <string>
#include <memory>

#include <atlasdb/db/context.h>
#include <atlasdb/query/engine.h>

namespace atlasdb {
  namespace db {

    using std::string;
    using boost::property_tree::ptree;

    struct db_data {
      query::engine_ptr engine;
    };

    class db {
    public:

      db() {}

      db(std::nullptr_t) {}

      db(db&& db) : _pimpl(std::move(db._pimpl)) {}

    public:

      // api for es
      /*
       * Indicate whether the given table exists
       * */
      bool exists(const string& table);

      /*
       * Create an empty table
       * */
      void create(const string& table);
      void create(const string& table, const std::vector<string>& indexes);

      /*
       * Drop an table, including all indexes
       * */
      void drop(const string& table);

      /*
       * Make an table empty
       * */
      void truncate(const string& table);

      /*
       * Make a duplication
       * */
      void duplicate(const string& table, const string& new_es);

      /*
       * Check for errors, reserved
       * */
      bool validate(const string& table);

      /**
       * Repair the table if possible, reserved
       * */
      void repair(const string& table);

      void count(const string& table);

      void load(const string& table);
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

    private:

      std::shared_ptr<db_data> _pimpl;
    };
  } // db
} // atlasdb

#endif // atlasdb_DB_DB_H_
