#ifndef BASIC_STOREHOUSE_H_
#define BASIC_STOREHOUSE_H_

#include <atlasdb/storage/basic_repository.h>
#include <atlasdb/storage/basic_indexer.h>

namespace atlasdb {
  namespace storage {

    /*
     * A IndexedStorage presents a large local pool of carefully organized
     * key/value pairs, besides indices to this pool.
     * The pool can be either in the memory or persisted to the disk
     * @see Storage
     * */
    template<typename Key, typename Value, typename Ikey>
    class basic_storehouse:
      public basic_repository<Key, Value, Ikey>,
      public basic_indexer<Key, Value, Ikey> {

    public:

      typedef basic_storehouse<Key, Value, Ikey> self_type;

      typedef basic_repository<Key, Value, Ikey> repository;
      typedef basic_indexer<Key, Value, Ikey> indexer;
      typedef basic_index<Key, Value, Ikey> index;

      typedef typename Key key_type;
      typedef typename Value value_type;
      typedef typename std::pair<key_type, value_type> node_type;
      typedef typename std::less<key_type> key_compare;

      typedef typename Ikey index_key_type;
      typedef typename Key index_value_type;
      typedef typename std::pair<index_key_type, index_value_type> index_node_type;
      typedef typename std::less<index_key_type> index_key_compare;

      typedef std::vector<key_type> keyset; // use vector instead of set for better performance
      typedef std::vector<index_key_type> ikeyset; // use vector instead of set for better performance
      typedef std::map<string, ikey_set> named_ikeyset;

      typedef size_t identifier;

    public:

      basic_storehouse(const string name);

      basic_storehouse(const string name, const std::vector<string>& indexes);

      virtual ~basic_storehouse() {}

    public:

      void open(const string name, const std::vector<string>& indexes);

      virtual void close();

      /**
       * insert a data node to the storage, all relative indexes are also updated
       */
      void put(const key_type& key, const data_type& data, const named_ikeyset& ikeys);

      void put(const value_type& node, const named_ikeyset& ikeys);

      /**
       * @delay, i am not sure to provider such method
       */
      template<class Key2, class Value2>
      void put(const Key2& key, const Value2& value);

      /**
      * @delay, i am not sure to provider such method
      */
      template<class Node>
      void put(const Node& node);

      /**
       * update a data node in the storage, all relative indexes are also updated
       */
      void update(const key_type& key, const data_type& data, const named_ikeyset& ikeys);

      void update(const value_type& value, const named_ikeyset& ikeys);

      template<class Key2, class Value2>
      void update(const Key2& key, const Value2& data);

      template<class Key2, class Value2>
      void update(const Key2& key, const Key2& new_key, const Value2& new_data);

      /**
       * @delay, i am not sure to provider such method
       */
      template<class Key, class Node>
      void update(const Key& key, const Node& node);

      /**
       * delete an existing data node in the storage, all relative indexes
       * are also updated
       */
      void del(const key_type& key, const named_ikeyset& ikeys);

    private:

      std::error_code err_code(atlasdb::storage::errc e) {
        return std::error_code(static_cast<int>(e), storage_error_category::instance());
      }
    };

  } // storage
} // atlasdb

#endif
