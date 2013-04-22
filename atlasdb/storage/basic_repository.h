#ifndef BASIC_REPOSITORY_H_
#define BASIC_REPOSITORY_H_

#include <atlasdb/storage/btree/btree_map.h>

#include <atlasdb/storage/storage_base.h>
#include <atlasdb/storage/basic_environment.h>
#include <atlasdb/storage/basic_transaction.h>

namespace atlasdb {
  class raw_block;
  template<class > class rd_wrapper;
}

namespace atlasdb {
  namespace storage {

    using std::string;

    /**
     * A Storage presents a large amount of carefully organized key/value pairs
     * The key/value pairs can be either in the memory or persisted in the disk
     * */
    template<typename Key, typename Value, typename Ikey>
    class basic_repository : virtual public storage_base {
    public:

      typedef typename Key key_type;
      typedef typename Value value_type;
      typedef typename std::pair<key_type, value_type> node_type;
      typedef typename std::less<key_type> key_compare;

      typedef size_t identifier;

      typedef storage_base base_type;
      typedef basic_repository<Key, Value, Ikey> self_type;

    public:

      static void create(const string& table);
      static void drop(const string& table);

    public:

      basic_repository() = default;

      basic_repository(const string& table) :
          base(table) {
        open(table);
      }

      virtual ~basic_repository() {
        close();
      }

    public:

      /**
       * open a primary storage for reading and writing
       * @return true if success
       */
      bool open(const string& table);

      /**
       * close this storage
       * @throw throw an exception if failed
       */
      virtual void close();

      bool upper_bound(const key_type& key, bool open);

      bool lower_bound(const key_type& key, bool open);

      /**
       * put data nodes to the storage
       * @return true if success
       * @notice these putting operation affects only the primary storage
       */
      bool put(const key_type& key, const value_type& data);
      bool put(const value_type& node);
      // the first <buf, sz> pair is the key and the second one is the value
      bool put(void* buf, size_t sz, void* buf2, size_t sz2);

      template<class Key, class Data>
      bool put(const Key& key, const Data& data) {
        rd_wrapper<Key> rk(key);
        rd_wrapper<Data> rd(data);
        return put(rk.address(), rk.size(), rd.address(), rd.size());
      }

      template<class Node>
      bool put(const Node& node) {
        typedef typename Node::key_type key_type;
        typedef typename Node::value_type value_type;

        rd_wrapper<key_type> rk(node.key());
        rd_wrapper<value_type> rd(node.value());
        return put(rk.address(), rk.size(), rd.address(), rd.size());
      }

      /**
       * Simple fetch a data block with a given key
       */
      boost::optional<value_type> get(const key_type& key) const;

      /**
       * Fetch a data block with the given key from the storage,
       * and store the data block into the given buffer
       *
       * @param key indicates which data block should be load
       * @param buffer the users provided buffer to store the data block
       * @param size the size of the buffer
       * @return reference to the loaded data block
       */
      value_type get(const key_type& key, void* buffer, size_t size) const;

      bool exists(const key_type& key) const;

      /**
       *  Replace a data block to the given data block
       *  All the open index should also be updated
       *  Note : how to know the nested fields?
       * @notice these updating operation affects only the primary storage
       */
      void update(const key_type& key, const value_type& new_value) {
        _container.find_unique(key);
      }

      void update(const key_type& key, const value_type& new_value, ptrdiff_t offset, size_t size);

      // delete the old one and insert a new one
      //void update(const key_type& key, const key_type& new_key, const value_type& data);
      //if really need template fuction? At least,the provider not suport method as "update(adress,size)"
      template<class Key2, class Value2>
      void update(const Key2& key, const Value2& data);

      // delete the old one and insert a new one
      template<class Key2, class Value2>
      void update(const Key2& key, const Key2& new_key, const Value2& new_value);

      /**
       * delayed
       * @param P is a filed extractor used to indicate the exact field portion
       *    to be updated.
       */
      template<class P>
      void update(const key_type& key, const value_type& data, P p);

      /**
       * delete an existing data node in the storage
       * @notice these updating operation affects only the primary storage
       */
      void del(const key_type& key) {
        _container.erase(key);
      }

    public:

      size_t count() const {
        return _container.count();
      }

    public:

      // BidirectionalIterater
      // iterate over the records as a bidirectional list
      // Note : A storage must be an associated container just like
      // std::map, so the iterator interface can refers to std::map iterator
      // and berkeley db provider something similar
      typedef btree_map<key_type, value_type>::iterator iterator;
      typedef btree_map<key_type, value_type>::const_iterator const_iterator;
      typedef btree_map<key_type, value_type>::reverse_iterator reverse_iterator;
      typedef btree_map<key_type, value_type>::const_reverse_iterator const_reverse_iterator;

      bool empty() const {
        return _container.empty();
      }

      iterator begin() {
        return _container.begin();
      }

      const_iterator begin() const {
        return _container.begin();
      }

      iterator end() {
        return _container.end();
      }

      const_iterator end() const {
        return _container.end();
      }

      reverse_iterator rbegin() {
        return _container.rbegin();
      }

      const_reverse_iterator rbegin() const {
        return _container.rbegin();
      }

      reverse_iterator rend() {
        return _container.rbegin();
      }

      const_reverse_iterator rend() const {
        return _container.rend();
      }

      value_type front() {
        return _container.front();
      }

      const value_type front() const {
        return _container.front();
      }

      value_type back() {
        return _container.back();
      }

      const value_type back() const {
        return _container.back();
      }

    private:

      btree_map<key_type, value_type> _container;
    };

  } // storage
} // atlasdb

#endif // BASIC_STORAGE_H_
