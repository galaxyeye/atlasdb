/**
* attacher.h
*
*  Created on: Oct 9, 2011
*      Author: vincent
*/

#ifndef QUERY_ENTITY_H_
#define QUERY_ENTITY_H_

#include <utility>
#include <string>
#include <atlasdb/serialization/cson/BSONObj.h>

namespace atlasdb {
  namespace db {
    using std::pair;
    using std::make_pair;
    using std::string;

    using cson::CSONElement;
    using cson::BSONObj;

    class field {
    public:
      field(const string& path) : _path(path) {}
      string path() const { return path; }

    private:
      string      _path;
    };

    class index {
    public:

      class calculator {
      public:
        bool is_js() const;

        pair<const char*, size_t> operator()(ptrdiff_t offset, size_t sz) {
          const char* p = _data + offset;
          return make_pair<const char*, size_t>(p, sz);
        }

      private:
        pair<const char*, size_t> _data;
      };

    public:
      string name() const;
      bool is_primary() const;
      bool is_unique() const;
      calculator calculator() const {
        return _cal;
      }

    private:
      string _name;
      bool _is_primary;
      bool _is_unique;
      calculator _cal;
    };

    class transformer {
    public:
      bool is_js() const;

      pair<const char*, size_t> operator()(ptrdiff_t offset, size_t sz) {
        return make_pair<const char*, size_t>(_data + offset, sz);
      }

      pair<const char*, size_t> operator()(pair<const char*, size_t> target) {
        return target;
      }

      CSONElement operator()(string index) {
        BSONObj c(_data);
        return c.getElement(index);
      }

    private:
      pair<const char*, size_t> _data;
    };
  }
} // namespace atlasdb

#endif
