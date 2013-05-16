#ifndef BASE_INDEXER_IMPLE_H_
#define BASE_INDEXER_IMPLE_H_

namespace atlasdb {
  namespace storage {

    template<typename Key, typename Value, typename IKey>
    void basic_indexer<Key, Value, IKey>::create(const std::string& name) {
    }

    template<typename Key, typename Value, typename IKey>
    void basic_indexer<Key, Value, IKey>::drop(const std::string& name) {
    }

    template<typename Key, typename Value, typename IKey>
    typename basic_indexer<Key, Value, IKey>::index&
    basic_indexer<Key, Value, IKey>::open(const std::string& name) {
      return _indexes[name];
    }

    template<typename Key, typename Value, typename IKey>
    typename basic_indexer<Key, Value, IKey>::index&
    basic_indexer<Key, Value, IKey>::
    open(const std::string& name, const index_key_type& lower, bool open, const index_key_type& upper, bool open2) {
      return _indexes[name];
    }

    template<typename Key, typename Value, typename IKey>
    void basic_indexer<Key, Value, IKey>::close(index& index) {

    }

    template<typename Key, typename Value, typename IKey>
    void basic_indexer<Key, Value, IKey>::close() {

    }

  } // storage
} // atlas

#endif
