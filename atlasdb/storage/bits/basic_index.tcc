#ifndef BASE_INDEX_IMPLE_H_
#define BASE_INDEX_IMPLE_H_

namespace atlasdb {
  namespace storage {
//
//    template<class Provider>
//    typename basic_index<Provider>::identifier basic_index<Provider>::getindexid() {
//      return _id;
//    }
//    template<class Provider>
//    bool basic_index<Provider>::setlowerbound(const index_key_type& lower, bool open_interval) {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      assert(_id != 0);
//      if (p->lower_bound(_id, lower, open_interval)) {
//        return true;
//      }
//      throw storage_error(p->err_code());
//    }
//    template<class Provider>
//    bool basic_index<Provider>::setupperbound(const index_key_type& upper, bool open_interval) {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      assert(_id != 0);
//      if (p->upper_bound(_id, upper, open_interval)) {
//        return true;
//      }
//      throw storage_error(p->err_code());
//    }
//
//    template<class Provider>
//    void basic_index<Provider>::settxn(basic_transaction<txn_type>& txn) {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      assert(_id != 0);
//      p->idx_settxn(_id, txn.gettxn());
//    }
//
//    template<class Provider>
//    void basic_index<Provider>::cleantxn() {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      assert(_id != 0);
//      p->idx_cleantxn(_id);
//    }
//
//    template<class Provider>
//    bool basic_index<Provider>::get(const index_key_type& key, index_data_type& data) {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      assert(_id != 0);
//      if (!p->idx_get(_id, key, data)) {
//        std::error_code code = p->err_code();
//        if (code != errc::item_not_found) throw storage_error(code);
//        return false;
//      }
//      return true;
//    }
//
//    template<class Provider>
//    void basic_index<Provider>::insert(const index_key_type& key, const index_data_type& data) {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      assert(_id != 0);
//      p->idx_insert(_id, key, data);
//      std::error_code code = p->err_code();
//      if (code != errc::success) throw storage_error(p->err_code());
//    }
//
//    template<class Provider>
//    void basic_index<Provider>::update(const index_key_type& key, const index_key_type& new_key,
//        const index_data_type& data) {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      assert(_id != 0);
//      p->idx_update(_id, key, new_key, data);
//      std::error_code code = p->err_code();
//      if (code != errc::success) throw storage_error(p->err_code());
//    }
//
//    template<class Provider>
//    void basic_index<Provider>::del(const index_key_type& key) {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      assert(_id != 0);
//      p->idx_del(_id, key);
//      std::error_code code = p->err_code();
//      if (code != errc::success) throw storage_error(p->err_code());
//    }
//
//    template<class Provider>
//    void basic_index<Provider>::del(const index_key_type& key, const index_data_type& data) {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      assert(_id != 0);
//      p->idx_del(_id, key, data);
//      std::error_code code = p->err_code();
//      if (code != errc::success) throw storage_error(p->err_code());
//    }
//
//    template<class Provider>
//    bool basic_index<Provider>::empty() const {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      assert(_id != 0);
//      identifier csrid;
//      std::error_code code;
//      if (p->csr_open(_id) == 0) {
//        code = p->err_code();
//        throw storage_error(code);
//      }
//      p->csr_first(_id);
//      code = p->err_code();
//      p->csr_close(_id, csrid);
//      if (code == errc::success) return false;
//      else if (code == errc::item_not_found) return true;
//      else {
//        throw storage_error(code);
//      }
//    }
//    template<class Provider>
//    typename basic_index<Provider>::iterator basic_index<Provider>::begin() {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      iterator iter(p, _id, _getkey);
//      iter.begin();
//      return iter;
//    }
//
//    template<class Provider>
//    typename basic_index<Provider>::const_iterator basic_index<Provider>::begin() const {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      const_iterator iter(p, _id, _getkey);
//      iter.begin();
//      return iter;
//    }
//    template<class Provider>
//    typename basic_index<Provider>::iterator basic_index<Provider>::end() {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      iterator iter(p, _id, _getkey);
//      iter.end();
//      return iter;
//    }
//    template<class Provider>
//    typename basic_index<Provider>::const_iterator basic_index<Provider>::end() const {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      const_iterator iter(p, _id, _getkey);
//      iter.end();
//      return iter;
//    }
//
//    template<class Provider>
//    typename basic_index<Provider>::iterator basic_index<Provider>::find(const index_key_type& key) {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      iterator iter(p, _id, _getkey);
//      iter.find(key);
//      return iter;
//    }
//
//    template<class Provider>
//    typename basic_index<Provider>::const_iterator basic_index<Provider>::find(const index_key_type& key) const {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      const_iterator iter(p, _id, _getkey);
//      iter.find(key);
//      return iter;
//    }
//
//    template<class Provider>
//    typename basic_index<Provider>::reverse_iterator basic_index<Provider>::rbegin() {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      iterator iter(p, _id, _getkey);
//      iter.end();
//      reverse_iterator reverse_it(iter);
//      return reverse_it;
//    }
//    template<class Provider>
//    typename basic_index<Provider>::const_reverse_iterator basic_index<Provider>::rbegin() const {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      const_iterator iter(p, _id, _getkey);
//      iter.end();
//      const_reverse_iterator reverse_it(iter);
//      return reverse_it;
//    }
//
//    template<class Provider>
//    typename basic_index<Provider>::reverse_iterator basic_index<Provider>::rend() {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      iterator iter(p, _id, _getkey);
//      iter.begin();
//      reverse_iterator reverse_it(iter);
//      return reverse_it;
//    }
//    template<class Provider>
//    typename basic_index<Provider>::const_reverse_iterator basic_index<Provider>::rend() const {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      const_iterator iter(p, _id, _getkey);
//      iter.begin();
//      const_reverse_iterator reverse_it(iter);
//      return reverse_it;
//    }
//    template<class Provider>
//    typename basic_index<Provider>::index_value_type basic_index<Provider>::front() {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      iterator iter(p, _id, _getkey);
//      iter.begin();
//      return std::move(*iter);
//    }
//
//    template<class Provider>
//    const typename basic_index<Provider>::index_value_type basic_index<Provider>::front() const {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      const_iterator iter(p, _id, _getkey);
//      iter.begin();
//      return *iter;
//    }
//
//    template<class Provider>
//    typename basic_index<Provider>::index_value_type basic_index<Provider>::back() {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      iterator iter(p, _id, _getkey);
//      iter.end();
//      reverse_iterator reverse_it(iter);
//      return std::move(*reverse_it);
//    }
//    template<class Provider>
//    const typename basic_index<Provider>::index_value_type basic_index<Provider>::back() const {
//      Provider *p = get_provider();
//      assert(p != nullptr);
//      const_iterator iter(p, _id, _getkey);
//      iter.end();
//      const_reverse_iterator reverse_it(iter);
//      return *reverse_it;
//    }
//
//    template<class Provider>
//    bool basic_index<Provider>::operator==(const self& other) const {
//      return ((get_provider() == other.get_provider()) && (_id == other.id));
//    }
//
//    template<class Provider>
//    bool basic_index<Provider>::operator!=(const self& other) const {
//      return ((get_provider() != other.get_provider()) || (_id != other.id));
//    }

  } // storage
} // atlasdb

#endif // BASE_INDEX_IMPLE_H_
