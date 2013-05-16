/*
 * db.ipp
 *
 *  Created on: May 15, 2013
 *      Author: vincent
 */

#ifndef ATLASDB_DB_DB_IPP_
#define ATLASDB_DB_DB_IPP_

#include <atlasdb/query/stomanip.h>

namespace atlasdb {
  namespace db {

    using std::string;
    using std::stringbuf;
    using boost::property_tree::ptree;
    using query::stomanip;

    namespace query {
      class engine_ptr;
    }

    namespace {
      class __db {

        __db() {}

      public:

        void count(const string& table) {
          stomanip::count(table);
        }

        void load(const string& table) {
          _buffer = stomanip::load(table);
        }

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

        void insert(const ptree& q) {

        }

        void insert_pkey_value(const string& table, const string& pkey, const string& value);

        void remove(const ptree& q);
        void remove_by_pkey(const string& table, const string& pkey);
        void remove_by_key(const string& table, const string& index, const string& key);

      private:

        stringbuf _buffer;
        query::engine_ptr _engine;
      };
    } // anonymous

    db::db() {}

    db::db(std::nullptr_t) {}

    db::db(db::db&& db) : _pimpl(std::move(db._pimpl)) {}

    bool db::exists(const string& table);
    void db::create(const string& table);
    void db::create(const string& table, const std::vector<string>& indexes);
    void db::drop(const string& table);
    void db::truncate(const string& table);
    void db::duplicate(const string& table, const string& new_es);
    bool db::validate(const string& table);
    void db::repair(const string& table);

    void db::count(const string& table) { _pimpl->count(table); }

    void db::load(const string& table) { _pimpl->load(table); }

    void db::load_by_pkey(const string& table, const string& pkey)
    { _pimpl->load_by_pkey(table, pkey); }

    void db::load_by_key(const string& table, const string& index, const string& pkey)
    { _pimpl->load_by_key(table, index, pkey); }

    void db::load_field_by_pkey(const string& table, const string& pkey)
    { _pimpl->load_field_by_pkey(table, pkey); }

    void db::load_field_by_key(const string& table, const string& index, const string& pkey)
    { _pimpl->load_field_by_key(table, index, pkey); }

    void db::find(const ptree& q) { _pimpl->find(q); }

    void db::update(const ptree& q) { _pimpl->update(q); }

    void db::update_by_pkey(const string& table, const string& pkey)
    { _pimpl->update_by_pkey(table, pkey); }

    void db::update_by_key(const string& table, const string& index, const string& pkey)
    { _pimpl->update_by_key(table, index, pkey); }

    void db::update_field_by_pkey(const string& table, const string& pkey)
    { _pimpl->update_field_by_pkey(table, pkey); }

    void db::update_field_by_key(const string& table, const string& index, const string& pkey)
    { _pimpl->update_field_by_key(table, index, pkey); }

    void db::insert(const ptree& q) { _pimpl->insert(q); }
    void db::insert_pkey_value(const string& table, const string& pkey, const string& value)
    { _pimpl->insert_pkey_value(table, pkey, value); }

    void db::remove(const ptree& q) { _pimpl->remove(q); }

    void db::remove_by_pkey(const string& table, const string& pkey)
    { _pimpl->remove_by_pkey(table, pkey); }

    void db::remove_by_key(const string& table, const string& index, const string& key)
    { _pimpl->remove_by_key(table, index, key); }

  } // net
} // atlasdb

#endif /* ATLASDB_DB_DB_IPP_ */
