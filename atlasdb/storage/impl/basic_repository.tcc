#ifndef BASIC_REPOSITORY_IMPLE_H_
#define BASIC_REPOSITORY_IMPLE_H_

#include <cassert>

namespace atlas {
  namespace storage {

    template<typename Key, typename Value, typename IndexKey>
    void basic_repository<Key, Value, IndexKey>::create(const string& entityset) {
      Provider::create(entityset);
    }

    template<typename Key, typename Value, typename IndexKey>
    void basic_repository<Key, Value, IndexKey>::drop(basic_environment<env_type, txn_type>& env,
        const string& entityset) {
      Provider::drop(entityset, env.getenv());
    }

    template<typename Key, typename Value, typename IndexKey>
    bool basic_repository<Key, Value, IndexKey>::open(const string& entityset) {
      base::name(entityset);
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      if (p->is_open()) return true;
      if (p->open(entityset)) return true;
      throw storage_error(p->err_code());
    }

    template<typename Key, typename Value, typename IndexKey>
    bool basic_repository<Key, Value, IndexKey>::open(basic_environment& env, const string& entityset) {
      base::name(entityset);
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      if (p->is_open()) return true;
      if (p->open(entityset, env.getenv())) return true;
      throw storage_error(p->err_code());
    }
    template<typename Key, typename Value, typename IndexKey>
    bool basic_repository<Key, Value, IndexKey>::open(basic_environment& env, const string& entityset, int key_type) {
      base::name(entityset);
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      if (p->is_open()) return true;
      if (p->open(entityset, env.getenv(), key_type)) return true;
      throw storage_error(p->err_code());
    }

    template<typename Key, typename Value, typename IndexKey>
    bool basic_repository<Key, Value, IndexKey>::upper_bound(const key_type& key, bool flag) {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      if (p->upper_bound(key, flag)) return true;
      throw storage_error(p->err_code());
    }

    template<typename Key, typename Value, typename IndexKey>
    bool basic_repository<Key, Value, IndexKey>::lower_bound(const key_type& key, bool flag) {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      if (p->lower_bound(key, flag)) return true;
      throw storage_error(p->err_code());
    }

    template<typename Key, typename Value, typename IndexKey>
    void basic_repository<Key, Value, IndexKey>::close() {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      p->close();
      std::error_code code = p->err_code();
      if (code != errc::success) throw storage_error(code);
    }

    template<typename Key, typename Value, typename IndexKey>
    bool basic_repository<Key, Value, IndexKey>::put(const key_type& key, const value_type& data) {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      if (p->put(key, data)) return true;
      throw storage_error(p->err_code());
    }

    template<typename Key, typename Value, typename IndexKey>
    bool basic_repository<Key, Value, IndexKey>::put(const value_type& node) {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      if (p->put(node)) return true;
      throw storage_error(p->err_code());
    }

    template<typename Key, typename Value, typename IndexKey>
    bool basic_repository<Key, Value, IndexKey>::put(void* buf, size_t sz, void* buf2, size_t sz2) {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      if (p->put(buf, sz, buf2, sz2)) return true;
      throw storage_error(p->err_code());
    }

    template<typename Key, typename Value, typename IndexKey>
    bool basic_repository<Key, Value, IndexKey>::get(const key_type& key, value_type& data) const {
      value_type value;
      value.first = key;

      Provider *p = this->get_provider();
      assert(p != nullptr);
      if (p->get(key, data)) {
        return true;
      }
      std::error_code code = p->err_code();
      if (code != errc::item_not_found) throw code;
      return false;
    }

    template<typename Key, typename Value, typename IndexKey>
    typename basic_repository<Key, Value, IndexKey>::value_type basic_repository<Key, Value, IndexKey>::get(
        const key_type& key, void* buffer, size_t size) const {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      value_type node = p->get(key, buffer, size);
      std::error_code code = p->err_code();
      if (code == errc::success) return node;
      if (code != errc::item_not_found) throw code;
      return node;
    }
    template<typename Key, typename Value, typename IndexKey>
    bool basic_repository<Key, Value, IndexKey>::get_bulk(char* outdata, size_t size) {
      Provider *p = base::get_provider();
      assert(p != nullptr);
      if (p->get_bulk(outdata, size)) return true;
      std::error_code code = p->err_code();
      if (code == errc::item_not_found) return false;
      throw code;
    }

    template<typename Key, typename Value, typename IndexKey>
    void basic_repository<Key, Value, IndexKey>::del(const key_type& key) {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      p->del(key);
      std::error_code code = p->err_code();
      if (code != errc::success) throw code;
      return;
    }

    template<typename Key, typename Value, typename IndexKey>
    void basic_repository<Key, Value, IndexKey>::update(const key_type& key, const value_type& new_data) {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      p->update(key, new_data);
      std::error_code code = p->err_code();
      if (code != errc::success) throw code;
    }

    template<typename Key, typename Value, typename IndexKey>
    void basic_repository<Key, Value, IndexKey>::update(const key_type& key, const value_type& data, ptrdiff_t offset,
        size_t size) {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      p->update(key, data, offset, size);
      std::error_code code = p->err_code();
      if (code != errc::success) throw code;
    }

    // delete the old one and insert a new one
    /*template<typename Key, typename Value, typename IndexKey>
     void basic_repository<Key, Value, IndexKey>::
     update(const key_type& key, const key_type& new_key, const value_type& data){
     Provider *p = base::get_provider();
     assert(p!=nullptr);
     p->idx_update(key, new_key, data);
     }*/

    template<typename Key, typename Value, typename IndexKey>
    template<class Key, class Data>
    void basic_repository<Key, Value, IndexKey>::update(const Key& key, const Data& data) {
      atlas::rd_wrapper < Key > rk(key);
      atlas::rd_wrapper < Data > rd(data);
      key_type k(rk.address(), rk.size());
      value_type d(rd.address(), rd.size());
      update(k, d);
    }

    // delete the old one and insert a new one
    template<typename Key, typename Value, typename IndexKey>
    template<class Key, class Data>
    void basic_repository<Key, Value, IndexKey>::update(const Key& key, const Key& new_key, const Data& data) {
      atlas::rd_wrapper < Key > rk(key);
      atlas::rd_wrapper < Key > rnk(new_key);
      atlas::rd_wrapper < Data > rd(data);
      key_type k(rk.address(), rk.size());
      key_type newk(rnk.address(), rnk.size());
      value_type d(rd.address(), rd.size());
      update(k, newk, d);
    }

    template<typename Key, typename Value, typename IndexKey>
    bool basic_repository<Key, Value, IndexKey>::exists(const key_type& key) const {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      if (p->exists(key)) {
        return true;
      }
      std::error_code code = p->err_code();
      if (code != errc::item_not_found) throw code;
      return false;

    }

    template<typename Key, typename Value, typename IndexKey>
    void basic_repository<Key, Value, IndexKey>::settxn(basic_transaction<txn_type>& txn) {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      p->settxn(txn.gettxn());
      return;
    }

    template<typename Key, typename Value, typename IndexKey>
    void basic_repository<Key, Value, IndexKey>::cleantxn() {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      p->cleantxn();
      return;
    }

    template<typename Key, typename Value, typename IndexKey>
    bool basic_repository<Key, Value, IndexKey>::empty() const {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      typename Provider::identifier csrid;
      std::error_code code;
      if ((csrid = p->csr_open()) == 0) {
        code = p->err_code();
        throw storage_error(code);
      }
      key_type key;
      value_type data;
      p->csr_next(csrid, key, data);
      code = p->err_code();
      p->csr_close(csrid);
      if (code == errc::success) return false;
      else if (code == errc::item_not_found) return true;
      else {
        throw storage_error(code);
      }
    }

    template<typename Key, typename Value, typename IndexKey>
    typename basic_repository<Key, Value, IndexKey>::iterator basic_repository<Key, Value, IndexKey>::begin() {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      iterator iter(p, 0);
      iter.begin();
      return iter;
    }

    template<typename Key, typename Value, typename IndexKey>
    typename basic_repository<Key, Value, IndexKey>::iterator basic_repository<Key, Value, IndexKey>::end() {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      iterator iter(p, 0);
      iter.end();
      return iter;
    }

    template<typename Key, typename Value, typename IndexKey>
    typename basic_repository<Key, Value, IndexKey>::const_iterator basic_repository<Key, Value, IndexKey>::begin() const {
      Provider* p = base::get_provider();
      assert(p!=nullptr);
      const_iterator iter(p, 0);
      iter.begin();
      return iter;
    }

    template<typename Key, typename Value, typename IndexKey>
    typename basic_repository<Key, Value, IndexKey>::const_iterator basic_repository<Key, Value, IndexKey>::end() const {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      const_iterator iter(p, 0);
      iter.end();
      return iter;
    }

    template<typename Key, typename Value, typename IndexKey>
    typename basic_repository<Key, Value, IndexKey>::reverse_iterator basic_repository<Key, Value, IndexKey>::rbegin() {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      iterator iter(p, 0);
      iter.end();
      reverse_iterator reverse_it(iter);
      return reverse_it;
    }

    template<typename Key, typename Value, typename IndexKey>
    typename basic_repository<Key, Value, IndexKey>::reverse_iterator basic_repository<Key, Value, IndexKey>::rend() {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      iterator iter(p, 0);
      iter.begin();
      reverse_iterator reverse_it(iter);
      return reverse_it;
    }

    template<typename Key, typename Value, typename IndexKey>
    typename basic_repository<Key, Value, IndexKey>::const_reverse_iterator basic_repository<Key, Value, IndexKey>::rbegin() const {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      const_iterator iter(p, 0);
      iter.end();
      const_reverse_iterator reverse_it(iter);
      return reverse_it;
    }

    template<typename Key, typename Value, typename IndexKey>
    typename basic_repository<Key, Value, IndexKey>::const_reverse_iterator basic_repository<Key, Value, IndexKey>::rend() const {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      const_iterator iter(p, 0);
      iter.begin();
      const_reverse_iterator reverse_it(iter);
      return reverse_it;
    }

    template<typename Key, typename Value, typename IndexKey>
    typename basic_repository<Key, Value, IndexKey>::value_type basic_repository<Key, Value, IndexKey>::front() {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      iterator iter(p, 0);
      iter.begin();
      return std::move(*iter);
    }

    template<typename Key, typename Value, typename IndexKey>
    const typename basic_repository<Key, Value, IndexKey>::value_type basic_repository<Key, Value, IndexKey>::front() const {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      iterator iter(p, 0);
      iter.begin();
      return (*iter);
    }

    template<typename Key, typename Value, typename IndexKey>
    typename basic_repository<Key, Value, IndexKey>::value_type basic_repository<Key, Value, IndexKey>::back() {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      iterator iter(p, 0);
      iter.end();
      reverse_iterator reverse_it(iter);
      return std::move(*reverse_it);
    }

    template<typename Key, typename Value, typename IndexKey>
    const typename basic_repository<Key, Value, IndexKey>::value_type basic_repository<Key, Value, IndexKey>::back() const {
      Provider *p = base::get_provider();
      assert(p!=nullptr);
      const_iterator iter(p, 0);
      iter.end();
      const_reverse_iterator reverse_it(iter);
      return (*reverse_it);
    }

    template<typename Key, typename Value, typename IndexKey>
    bool basic_repository<Key, Value, IndexKey>::operator==(const self& other) const {
      return base::get_provider() == other.base::get_provider();
    }

    template<typename Key, typename Value, typename IndexKey>
    bool basic_repository<Key, Value, IndexKey>::operator!=(const self& other) const {
      return base::get_provider() != other.base::get_provider();
    }
  }
}
