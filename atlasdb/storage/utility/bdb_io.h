#ifndef STORAGE_BDB_IO_H_
#define STORAGE_BDB_IO_H_

#include <string>
#include <ostream>
#include <memory>

#include <atlasdb/storage/bdb_provider.h>

namespace atlasdb {
  namespace storage {
    namespace provider {

      using std::string;
      using std::ostream;

      ostream& operator<<(ostream& os, const bdb_key& key) {
        const char* s = static_cast<const char*>(key.address());

        os << std::string(s, key.size());

        return os;
      }

      ostream& operator<<(ostream& os, const bdb_data& data) {
        const char* s = static_cast<const char*>(data.address());

        os << std::string(s, data.size());

        return os;
      }
    } // namespace ir
  }  // namespace query
} // namespace atlasdb

#endif // QUERY_IR_IO_H_
