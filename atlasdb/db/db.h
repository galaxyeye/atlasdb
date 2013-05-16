/*
 * db.h
 *
 *  Created on: Jun 10, 2012
 *      Author: vincent
 */

#ifndef ATLASDB_DB_DB_H_
#define ATLASDB_DB_DB_H_

#include <string>
#include <memory>

#include <boost/property_tree/ptree.hpp>

namespace atlasdb {
  namespace db {

    using std::string;
    using boost::property_tree::ptree;

    namespace query {
      class engine_ptr;
    }

    namespace {
      class __db;
    }

    class db {
    public:

      db() {}
      db(std::nullptr_t) {}
      db(db&& db) : _pimpl(std::move(db._pimpl)) {}

    public:

      bool exists(const string& table);
      void create(const string& table);
      void create(const string& table, const std::vector<string>& indexes);
      void drop(const string& table);
      void truncate(const string& table);
      void duplicate(const string& table, const string& new_es);
      bool validate(const string& table);
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
      void insert_pkey_value(const string& table, const string& pkey, const string& value);

      void remove(const ptree& q);
      void remove_by_pkey(const string& table, const string& pkey);
      void remove_by_key(const string& table, const string& index, const string& key);

    private:

      std::shared_ptr<__db> _pimpl;
    };

  } // db
} // atlasdb

#endif // ATLASDB_DB_DB_H_
