/*
 * key.h
 *
 *  Created on: Apr 6, 2013
 *      Author: vincent
 */

#ifndef atlasdb_KEY_VALUE_H_
#define atlasdb_KEY_VALUE_H_

#include <utility> // pair
#include <string>
#include <array>

namespace atlasdb {
  namespace query {

    using std::string;

    class key {
    public:

      key(const char* k, size_t sz);
      key(const string& k);
      key(const key&);
      key(key&&);

      key& operator=(const key&);
      key& operator=(key&&);
      bool operator<(const key&);

      std::pair<const char*, size_t> data() const;
      void data(const char* k, size_t sz);
      const char* address() const;

      constexpr size_t size() const;
      constexpr size_t capacity() const;

    public:

      static const key nil;

    private:
    };

    static const key key::nil;

    class value {
    public:

      value(const char* k, size_t sz);
      value(const string& k);
      value(const value&);
      value(value&&);

      value& operator=(const value&);
      value& operator=(value&&);

      std::pair<const char*, size_t> data() const;
      void data(const char* k, size_t sz);
      const char* address() const;

      constexpr size_t size() const;
      constexpr size_t capacity() const;

    public:

      static const value nil;
    };

    static const value value::nil;

    class ikey : public key {
    };
  } // storage
} // atlasdb

#endif /* atlasdb_KEY_VALUE_H_ */
