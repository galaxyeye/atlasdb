#ifndef atlasdb_QUERY_STORAGE_FWD_H_
#define atlasdb_QUERY_STORAGE_FWD_H_

#include <memory>
#include <atlasdb/query/kv.h>
#include <atlasdb/storage/basic_storehouse.h>

namespace atlasdb {
  namespace query {

    typedef storage::basic_index<key, value, ikey> index;
    typedef storage::basic_indexer<key, value, ikey> indexer;
    typedef storage::basic_repository<key, value, ikey> repository;
    typedef storage::basic_storehouse<key, value, ikey> storehouse;

    typedef std::shared_ptr<index> index_ptr;
    typedef std::shared_ptr<indexer> indexer_ptr;
    typedef std::shared_ptr<repository> repository_ptr;
    typedef std::shared_ptr<storehouse> storehouse_ptr;
  } // query
} // atlasdb

#endif // atlasdb_QUERY_STORAGE_FWD_H_
