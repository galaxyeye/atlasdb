#ifndef BASIC_REPOSITORY_TCC_
#define BASIC_REPOSITORY_TCC_

#include <cassert>

namespace atlasdb {
  namespace storage {

    template<typename Key, typename Value, typename IKey>
    void basic_repository<Key, Value, IKey>::create(const std::string& table) {
    }

    template<typename Key, typename Value, typename IKey>
    void basic_repository<Key, Value, IKey>::drop(const std::string& table) {
    }

    template<typename Key, typename Value, typename IKey>
    bool basic_repository<Key, Value, IKey>::open(const std::string& table) {
      return false;
    }

    template<typename Key, typename Value, typename IKey>
    bool basic_repository<Key, Value, IKey>::upper_bound(const key_type& key, bool flag) {
      return false;
    }

    template<typename Key, typename Value, typename IKey>
    bool basic_repository<Key, Value, IKey>::lower_bound(const key_type& key, bool flag) {
      return false;
//      Provider *p = base::get_provider();
//      assert(p!=nullptr);
//      if (p->lower_bound(key, flag)) return true;
//      throw storage_error(p->err_code());
    }

    template<typename Key, typename Value, typename IKey>
    void basic_repository<Key, Value, IKey>::close() {
//      Provider *p = base::get_provider();
//      assert(p!=nullptr);
//      p->close();
//      std::error_code code = p->err_code();
//      if (code != errc::success) throw storage_error(code);
    }

    template<typename Key, typename Value, typename IKey>
    bool basic_repository<Key, Value, IKey>::put(const key_type& key, const value_type& value) {
      return false;
//      Provider *p = base::get_provider();
//      assert(p!=nullptr);
//      if (p->put(key, value)) return true;
//      throw storage_error(p->err_code());
    }

    template<typename Key, typename Value, typename IKey>
    bool basic_repository<Key, Value, IKey>::put(const value_type& node) {
      return false;
//      Provider *p = base::get_provider();
//      assert(p!=nullptr);
//      if (p->put(node)) return true;
//      throw storage_error(p->err_code());
    }

    template<typename Key, typename Value, typename IKey>
    bool basic_repository<Key, Value, IKey>::put(void* buf, size_t sz, void* buf2, size_t sz2) {
      return false;
//      Provider *p = base::get_provider();
//      assert(p!=nullptr);
//      if (p->put(buf, sz, buf2, sz2)) return true;
//      throw storage_error(p->err_code());
    }

    template<typename Key, typename Value, typename IKey>
    typename basic_repository<Key, Value, IKey>::value_type
    basic_repository<Key, Value, IKey>::get(const key_type& key, void* buffer, size_t size) const {
//      Provider *p = base::get_provider();
//      assert(p!=nullptr);
//      value_type node = p->get(key, buffer, size);
//      std::error_code code = p->err_code();
//      if (code == errc::success) return node;
//      if (code != errc::item_not_found) throw code;
//      return node;
    }

    template<typename Key, typename Value, typename IKey>
    void basic_repository<Key, Value, IKey>::update(const key_type& key, const value_type& value, ptrdiff_t offset,
        size_t size) {
//      Provider *p = base::get_provider();
//      assert(p!=nullptr);
//      p->update(key, value, offset, size);
//      std::error_code code = p->err_code();
//      if (code != errc::success) throw code;
    }

    // delete the old one and insert a new one
    /*template<typename Key, typename Value, typename IKey>
     void basic_repository<Key, Value, IKey>::
     update(const key_type& key, const key_type& new_key, const value_type& value){
     Provider *p = base::get_provider();
     assert(p!=nullptr);
     p->idx_update(key, new_key, value);
     }*/


    template<typename Key, typename Value, typename IKey>
    bool basic_repository<Key, Value, IKey>::exists(const key_type& key) const {
      return false;
//      Provider *p = base::get_provider();
//      assert(p!=nullptr);
//      if (p->exists(key)) {
//        return true;
//      }
//      std::error_code code = p->err_code();
//      if (code != errc::item_not_found) throw code;
//      return false;
    }

  }
}

#endif // BASIC_REPOSITORY_TCC_
