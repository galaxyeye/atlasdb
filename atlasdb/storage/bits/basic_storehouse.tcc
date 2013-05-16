#ifndef ATLAS_STORAGE_BASIC_STOREHOUSE_TCC_
#define ATLAS_STORAGE_BASIC_STOREHOUSE_TCC_

#include <cassert>

namespace atlasdb {
  namespace storage {

    template<typename Key, typename Value, typename Ikey>
    void basic_storehouse<Key, Value, Ikey>::close() {
      repository::close();
      indexer::close();
    }

    template<typename Key, typename Value, typename Ikey>
    void basic_storehouse<Key, Value, Ikey>::put(const key_type& key, const value_type& value, const named_ikeyset& ikeys) {
//      if (ikeys.size() != indexer::size()) {
//        throw storage_error(err_code(errc::indexnum_not_match));
//      }
//      repository::put(key, data);
//
//      if (!ikeys.empty()) {
//        index_value_type ikeys(key.address(), key.size());
//        auto ikey = ikeys.begin();
//        auto ikey_end = ikeys.end();
//        auto index = indexer::begin();
//        for (; ikey != ikey_end; ikey++, index++) {
//          index->second.insert(*ikey, ikeys);
//        }
//      }
    }

    template<typename Key, typename Value, typename Ikey>
    void basic_storehouse<Key, Value, Ikey>::put(const node_type& node, const named_ikeyset& ikeys) {
//      if (ikeys.size() != indexer::size()) {
//        throw storage_error(err_code(errc::indexnum_not_match));
//      }
//      repository::put(node);
//
//      if (!ikeys.empty()) {
//        index_value_type ikeys(node.first.address(), node.first.size());
//
//        auto ikey = ikeys.begin();
//        auto ikey_end = ikeys.end();
//        auto index = indexer::begin();
//        for (; ikey != ikey_end; ikey++, index++) {
//          index->second.insert(*ikey, ikeys);
//        }
//      }
    }

    template<typename Key, typename Value, typename Ikey>
    void basic_storehouse<Key, Value, Ikey>::update(const key_type& key, const value_type& value, const named_ikeyset& ikeys) {
//      repository::update(key, data);
//
//      index_value_type ikeys(key.address(), key.size());
//      auto ikey = ikeys.begin();
//      auto ikey_end = ikeys.end();
//      for (; ikey != ikey_end; ikey++) {
//        indexer::at(get < 0 > (*ikey)).update(get < 1 > (*ikey), get < 2 > (*ikey), ikeys);
//      }
    }

    template<typename Key, typename Value, typename Ikey>
    void basic_storehouse<Key, Value, Ikey>::update(const node_type& node, const named_ikeyset& ikeys) {
//      repository::update(node.frist, node.second);
//
//      index_value_type ikeys(node.frist.address(), node.frist.size());
//      auto ikey = ikeys.begin();
//      auto ikey_end = ikeys.end();
//
//      for (; ikey != ikey_end; ikey++) {
//        indexer::at(std::get < 0 > (*ikey)).update(std::get < 1 > (*ikey), std::get < 2 > (*ikey), ikeys);
//      }
    }

    template<typename Key, typename Value, typename Ikey>
    void basic_storehouse<Key, Value, Ikey>::del(const key_type& key, const named_ikeyset& ikeys) {
      // if (ikeys.size() != indexer::size()) throw storage_error(err_code(errc::indexnum_not_match));

//      repository::del(key);
//
//      index_value_type ikeys(key.address(), key.size());
//      auto ikey = ikeys.begin();
//      auto ikey_end = ikeys.end();
//      auto index = indexer::begin();
//      for (; ikey != ikey_end; ikey++, index++) {
//        index->second.del(*ikey, ikeys);
//      }
    }

  } // storage
} // atlas

#endif // ATLAS_STORAGE_BASIC_STOREHOUSE_TCC_
