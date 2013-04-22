#ifndef BASE_INDEXER_IMPLE_H_
#define BASE_INDEXER_IMPLE_H_

namespace atlas {
  namespace storage {

    template<class Provider>
    void basic_indexer<Key, Value, IndexKey>::create(const std::string& name) {
      Provider::idx_create(name);
    }

    template<class Provider>
    void basic_indexer<Key, Value, IndexKey>::drop(basic_environment<env_type, txn_type>& env, const std::string& name) {
      Provider::idx_drop(name, env);
    }

    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::index_type&
    basic_indexer<Key, Value, IndexKey>::open(const std::string& name, const index_key_type& lower, bool open_interval,
        const index_key_type& upper, bool open_interval2) {
      Provider* p = get_provider();
      assert(p != nullptr);

      identifier id = p->idx_open(name);
      if (id == 0) throw storage_error(p->err_code());
      index_type index(name, id, p);
      if (lower.address() != nullptr) index.setlowerbound(lower, open_interval);
      if (upper.address() != nullptr) index.setupperbound(upper, open_interval2);
      _indices.insert(std::make_pair(name, index));

      mmap_range rang = _indices.equal_range(name);
      --(rang.second);
      return (*(rang.second)).second;
    }

    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::index_type&
    basic_indexer<Key, Value, IndexKey>::open(basic_environment<env_type, txn_type>& env, const std::string& name,
        const index_key_type& lower, bool open_interval, const index_key_type& upper, bool open_interval2) {
      Provider* p = get_provider();
      assert(p != nullptr);

      identifier id = p->idx_open(name, env.getenv());
      if (id == 0) throw storage_error(p->err_code());
      index_type index(name, id, p);
      if (lower.address() != nullptr) index.setlowerbound(lower, open_interval);
      if (upper.address() != nullptr) index.setupperbound(upper, open_interval2);
      _indices.insert(std::make_pair(name, index));
      mmap_range rang = _indices.equal_range(name);
      --(rang.second);
      return (*(rang.second)).second;
    }
    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::index_type&
    basic_indexer<Key, Value, IndexKey>::open(basic_environment<env_type, txn_type>& env, const std::string& name, int key_type,
        const index_key_type& lower, bool open_interval, const index_key_type& upper, bool open_interval2) {
      Provider* p = get_provider();
      assert(p != nullptr);

      identifier id = p->idx_open(name, env.getenv(), key_type);
      if (id == 0) throw storage_error(p->err_code());
      index_type index(name, id, p);
      if (lower.address() != nullptr) index.setlowerbound(lower, open_interval);
      if (upper.address() != nullptr) index.setupperbound(upper, open_interval2);
      _indices.insert(std::make_pair(name, index));
      mmap_range rang = _indices.equal_range(name);
      --(rang.second);
      return (*(rang.second)).second;
    }

    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::index_type&
    basic_indexer<Key, Value, IndexKey>::open(const std::string& name) {
      // TODO: This can be optimized
      Provider* p = get_provider();
      assert(p != nullptr);

      identifier id = p->idx_open(name);
      if (id == 0) throw storage_error(p->err_code());
      index_type index(name, id, p);
      _indices.insert(std::make_pair(name, index));
      mmap_range rang = _indices.equal_range(name);
      --(rang.second);
      return (*(rang.second)).second;
    }

    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::index_type&
    basic_indexer<Key, Value, IndexKey>::open(basic_environment<env_type, txn_type>& env, const std::string& name) {
      // TODO: This can be optimized
      Provider* p = get_provider();
      assert(p != nullptr);

      identifier id = p->idx_open(name, env.getenv());
      if (id == 0) throw storage_error(p->err_code());
      index_type index(name, id, p);
      _indices.insert(std::make_pair(name, index));
      mmap_range rang = _indices.equal_range(name);
      --(rang.second);
      return (*(rang.second)).second;
    }
    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::index_type&
    basic_indexer<Key, Value, IndexKey>::open(basic_environment<env_type, txn_type>& env, const std::string& name, int key_type,
        bool getkey) {
      // TODO: This can be optimized
      Provider* p = get_provider();
      assert(p != nullptr);

      identifier id = p->idx_open(name, env.getenv(), key_type);
      if (id == 0) throw storage_error(p->err_code());
      index_type index(name, id, p, getkey);
      _indices.insert(std::make_pair(name, index));
      mmap_range rang = _indices.equal_range(name);
      --(rang.second);
      return (*(rang.second)).second;
    }

    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::index_type&
    basic_indexer<Key, Value, IndexKey>::operator[](const std::string& name) {
      if (_indices.count(name) != 0) throw storage_error(err_code(errc::invalid_argument));
      mmap_range rang = _indices.equal_range(name);
      return (*(rang.first)).second;
      //return _indices[name];
    }

    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::index_type&
    basic_indexer<Key, Value, IndexKey>::at(const std::string& name) {
      if (_indices.count(name) == 0) throw storage_error(err_code(errc::invalid_argument));
      mmap_range rang = _indices.equal_range(name);
      return (*(rang.first)).second;
      //return _indices[name];
    }

    template<class Provider>
    void basic_indexer<Key, Value, IndexKey>::close(index_type& index) {
      Provider* p = get_provider();
      assert(p != nullptr);
      p->idx_close(index.getindexid());
      std::error_code code = p->err_code();
      if (code != errc::success) throw storage_error(code);
      _indices.erase(index.name());
    }

    template<class Provider>
    void basic_indexer<Key, Value, IndexKey>::close() {
      Provider* p = get_provider();
      assert(p != nullptr);
      iterator iter;
      std::error_code code;
      for (iter = _indices.begin(); iter != _indices.end(); iter++) {
        p->idx_close((*iter).second.getindexid());
        code = p->err_code();
        if (code != errc::success) {
          throw storage_error(code);
        }
      }
      _indices.clear();
    }

    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::size_type basic_indexer<Key, Value, IndexKey>::max_size() const {
      return _indices.max_size();
    }
    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::size_type basic_indexer<Key, Value, IndexKey>::size() {
      return _indices.size();
    }

    template<class Provider>
    bool basic_indexer<Key, Value, IndexKey>::empty() const {
      return _indices.size() == 0;
    }

    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::iterator basic_indexer<Key, Value, IndexKey>::begin() {
      return _indices.begin();
    }

    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::const_iterator basic_indexer<Key, Value, IndexKey>::begin() const {
      return _indices.begin();
    }

    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::iterator basic_indexer<Key, Value, IndexKey>::end() {
      return _indices.end();
    }

    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::const_iterator basic_indexer<Key, Value, IndexKey>::end() const {
      return _indices.end();
    }

    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::reverse_iterator basic_indexer<Key, Value, IndexKey>::rbegin() {
      return _indices.rbegin();
    }

    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::const_reverse_iterator basic_indexer<Key, Value, IndexKey>::rbegin() const {
      return _indices.rbegin();
    }

    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::reverse_iterator basic_indexer<Key, Value, IndexKey>::rend() {
      return _indices.rend();
    }
    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::const_reverse_iterator basic_indexer<Key, Value, IndexKey>::rend() const {
      return _indices.rend();
    }
    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::value_type&
    basic_indexer<Key, Value, IndexKey>::front() {
      return _indices.front();
    }

    template<class Provider>
    const typename basic_indexer<Key, Value, IndexKey>::value_type&
    basic_indexer<Key, Value, IndexKey>::front() const {
      return _indices.front();
    }

    template<class Provider>
    typename basic_indexer<Key, Value, IndexKey>::value_type&
    basic_indexer<Key, Value, IndexKey>::back() {
      return _indices.back();
    }

    template<class Provider>
    const typename basic_indexer<Key, Value, IndexKey>::value_type&
    basic_indexer<Key, Value, IndexKey>::back() const {
      return _indices.back();
    }
  }
}
#endif
