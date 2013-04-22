#ifndef BASIC_storehouse_IMPLE_H_
#define BASIC_storehouse_IMPLE_H_

#include <cassert>

namespace atlas {
  namespace storage {

    template<class Provider>
    void basic_storehouse<Provider>::close() {
      repository_type::close();
      indexer_type::close();
    }

    template<class Provider>
    void basic_storehouse<Provider>::put(const key_type& key, const data_type& data,
        const vector<index_key_type>& indexkeys) {
      if (indexkeys.size() != indexer_type::size()) {
        throw storage_error(err_code(errc::indexnum_not_match));
      }
      repository_type::put(key, data);

      if (!indexkeys.empty()) {
        index_data_type indexdata(key.address(), key.size());
        auto indexkey = indexkeys.begin();
        auto indexkey_end = indexkeys.end();
        typename indexer_type::iterator index = indexer_type::begin();
        for (; indexkey != indexkey_end; indexkey++, index++) {
          (*index).second.insert(*indexkey, indexdata);
        }
      }
    }

    template<class Provider>
    void basic_storehouse<Provider>::put(const value_type& node, const vector<index_key_type>& indexkeys) {
      if (indexkeys.size() != indexer_type::size()) {
        throw storage_error(err_code(errc::indexnum_not_match));
      }
      repository_type::put(node);

      if (!indexkeys.empty()) {
        index_data_type indexdata(node.first.address(), node.first.size());

        auto indexkey = indexkeys.begin();
        auto indexkey_end = indexkeys.end();
        typename indexer_type::iterator index = indexer_type::begin();
        for (; indexkey != indexkey_end; indexkey++, index++) {
          (*index).second.insert(*indexkey, indexdata);
        }
      }
    }

    template<class Provider>
    void basic_storehouse<Provider>::update(const key_type& key, const data_type& data,
        const vector<tuple<string, index_key_type, index_key_type>>& indexkeys) {
      repository_type::update(key, data);

      index_data_type indexdata(key.address(), key.size());
      auto indexkey = indexkeys.begin();
      auto indexkey_end = indexkeys.end();
      for (; indexkey != indexkey_end; indexkey++) {
        indexer_type::at(get < 0 > (*indexkey)).update(get < 1 > (*indexkey), get < 2 > (*indexkey), indexdata);
      }
    }

    template<class Provider>
    void basic_storehouse<Provider>::update(const value_type& node,
        const vector<tuple<string, index_key_type, index_key_type>>& indexkeys) {
      repository_type::update(node.frist, node.second);

      index_data_type indexdata(node.frist.address(), node.frist.size());
      auto indexkey = indexkeys.begin();
      auto indexkey_end = indexkeys.end();

      for (; indexkey != indexkey_end; indexkey++) {
        indexer_type::at(std::get < 0 > (*indexkey)).update(std::get < 1 > (*indexkey), std::get < 2 > (*indexkey),
            indexdata);
      }
    }

    template<class Provider>
    void basic_storehouse<Provider>::del(const key_type& key, const vector<index_key_type>& indexkeys) {
      if (indexkeys.size() != indexer_type::size()) {
        throw storage_error(err_code(errc::indexnum_not_match));
      }
      repository_type::del(key);

      index_data_type indexdata(key.address(), key.size());
      auto indexkey = indexkeys.begin();
      auto indexkey_end = indexkeys.end();
      typename indexer_type::iterator index = indexer_type::begin();
      for (; indexkey != indexkey_end; indexkey++, index++) {
        (*index).second.del(*indexkey, indexdata);
      }
    }

    template<class Provider>
    void basic_storehouse<Provider>::settxnall(basic_transaction<txn_type>& txn) {
      repository_type::settxn(txn);

      typename indexer_type::iterator index_begin = indexer_type::begin();
      typename indexer_type::iterator index_end = indexer_type::end();
      for (; index_begin != index_end; ++index_begin) {
        (*index_begin).second.settxn(txn);
      }
    }
    template<class Provider>
    void basic_storehouse<Provider>::cleantxnall() {
      repository_type::cleantxn();

      typename indexer_type::iterator index_begin = indexer_type::begin();
      typename indexer_type::iterator index_end = indexer_type::end();
      for (; index_begin != index_end; ++index_begin) {
        (*index_begin).second.cleantxn();
      }
    }
  } // storage
} // atlas

#endif //BASE_STORAGE_IMPLE_H_
