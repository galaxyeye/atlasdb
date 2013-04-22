#ifndef BASIC_INDEXER_H_
#define BASIC_INDEXER_H_

#include <string>
#include <map>
#include <set>
#include <vector>
#include <memory>
#include <algorithm>

#include <atlasdb/storage/storage_error.h>
#include <atlasdb/storage/basic_index.h>
#include <atlasdb/storage/storage_base.h>

namespace atlasdb {
  namespace storage {
    class error_code;

    template<typename Key, typename Value, typename IndexKey>
    class basic_indexer: virtual public storage_base {
    public:

      typedef basic_indexer<Key, Value, IndexKey> self;
      typedef storage_base<Key, Value, IndexKey> base;
      typedef basic_index<Key, Value, IndexKey> index;

      typedef typename Key key_type;
      typedef typename Value value_type;
      typedef typename std::pair<key_type, value_type> node_type;
      typedef typename std::less<key_type> key_compare;

      typedef typename IndexKey index_key_type;
      typedef typename Key index_value_type;
      typedef typename std::pair<index_key_type, index_value_type> index_node_type;
      typedef typename std::less<index_key_type> index_key_compare;

      typedef typename size_t identifier;
      typedef size_t size_type;

    public:

      basic_indexer() : base() {}

      virtual ~basic_indexer() { close(); }

    public:

      static void create(const std::string& name);

      template<class InputIterator>
      static void create(InputIterator first, InputIterator last) {
        typedef typename InputIterator::value_type value_type;
        std::for_each(first, last, [](const value_type& name) {
          self::create(name);
        });
      }

      /**
       * Drop an exist index storage
       * @param name the name of the index storage
       **/
      static void drop(const std::string& name);

      /**
       * Drop indexes in a batch
       **/
      template<class InputIterator>
      static void drop(InputIterator first, InputIterator last) {
        typedef typename InputIterator::value_type value_type;
        std::for_each(first, last, [&env](const value_type& name) {
          self::drop(env, name);
        });
      }

    public:

      /**
       * open an index with an arithmetic range, one of the following cases:
       * [a, b], (a, b), [a, b), (a, b]
       * if none of the arguments is provided, it does a full scan over the
       * index storage
       *
       * @complex O(logN)
       * @param lower the lower bound
       * @param open indicate whether lower bound is an open interval(true)
       *          or an closed interval(false)
       * @param upper the upper bound
       * @param open2 indicate whether upper bound is an open interval(true)
       *          or an closed interval(false)
       * @return true if success
       */
      index& open(const std::string& name, const index_key_type& lower,
          bool open = true, const index_key_type& upper = index_key_type::nil,
          bool open2 = true);

      /**
       * open an index for a specified key
       */
      index& open(const std::string& name);

      index& open(const std::string& name, const index_key_type& lower, bool open = true,
          const index_key_type& upper = index_key_type::nil, bool open2 = true);

      index& open(const std::string& name);

      index& operator[](const std::string& name);

      index& at(const std::string& name);

      /**
       * close an open index
       * */
      virtual void close(index& index);

      /**
       * close all open index
       * */
      virtual void close();

    public:

      typedef typename std::multimap<std::string, index>::iterator iterator;
      typedef typename std::multimap<std::string, index>::const_iterator const_iterator;
      typedef typename std::multimap<std::string, index>::reverse_iterator reverse_iterator;
      typedef typename std::multimap<std::string, index>::const_reverse_iterator const_reverse_iterator;

      size_type max_size() const { return _indexes.max_size(); }

      size_type size() const { return _indexes.size(); }

      bool empty() const { return _indexes.empty(); }

      iterator begin() { return _indexes.begin(); }

      const_iterator begin() const { return _indexes.begin(); }

      iterator end() { return _indexes.begin(); }

      const_iterator end() const { return _indexes.end(); }

      reverse_iterator rbegin() { return _indexes.rbegin(); }

      const_reverse_iterator rbegin() const { return _indexes.rbegin(); }

      reverse_iterator rend() { return _indexes.rend(); }

      const_reverse_iterator rend() const { return _indexes.rend(); }

      value_type& front() { return _indexes.front(); }

      const value_type& front() const { return _indexes.front(); }

      value_type& back() { return _indexes.back(); }

      const value_type& back() const { return _indexes.back(); }

    private:

      std::multimap<std::string, index> _indexes;
    };
  } // storage
} // atlasdb

#endif // BASIC_INDEXER_H_
