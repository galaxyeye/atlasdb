#ifndef ATLASDB_STORAGE_BASIC_INDEX_H_
#define ATLASDB_STORAGE_BASIC_INDEX_H_

#include <string>
#include <boost/optional.hpp>

#include <atlasdb/storage/btree/btree_map.h>
#include <atlasdb/storage/storage_base.h>

namespace atlasdb {
  namespace storage {

    template<typename Key, typename Value, typename Ikey>
    class basic_index : public storage_base {
    public:

      typedef storage_base base_type;
      typedef basic_index<Key, Value, Ikey>  self_type;

      typedef Ikey index_key_type;
      typedef Key index_value_type;
      typedef std::pair<index_key_type, index_value_type> index_node_type;
      typedef std::less<index_key_type> index_key_compare;

    public:

      basic_index() = default;

      basic_index(const std::string& name) : base_type(name) {}

      virtual ~basic_index() {}

    public:

      bool lower_bound(const index_key_type& lower, bool open);

      bool upper_bound(const index_key_type& upper, bool open);

    protected:

      boost::optional<index_value_type> get(const index_key_type& key);

      void insert(const index_key_type& key, const index_value_type& data);

      void update(const index_key_type& key, const index_key_type& new_key, const index_value_type& data);

      void del(const index_key_type& key);

      void del(const index_key_type& key,const index_value_type& data);

    public:

      typedef typename btree_map<index_key_type, index_value_type>::iterator iterator;
      typedef typename btree_map<index_key_type, index_value_type>::const_iterator const_iterator;
      typedef typename btree_map<index_key_type, index_value_type>::reverse_iterator reverse_iterator;
      typedef typename btree_map<index_key_type, index_value_type>::const_reverse_iterator const_reverse_iterator;

      bool empty() const;
      iterator find(const index_key_type& key);
      const_iterator find(const index_key_type& key) const;
      iterator begin();
      const_iterator begin() const;
      iterator end();
      const_iterator end() const;
      reverse_iterator rbegin();
      const_reverse_iterator rbegin() const;
      reverse_iterator rend();
      const_reverse_iterator rend() const;
      index_value_type front();
      const index_value_type front() const;
      index_value_type back();
      const index_value_type back() const;

    private:

      btree_multimap<index_key_type, index_value_type, index_key_compare> _container;
    };

  } // storage
} // atlasdb

#endif // ATLASDB_STORAGE_BASIC_INDEX_H_
