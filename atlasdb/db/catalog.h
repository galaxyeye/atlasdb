/*
 * catalog.h
 *
 *  Created on: 2012-4-28
 *      Author: vincent
 */

#ifndef DB_CATALOG_H_
#define DB_CATALOG_H_

#include <atlasdb/db/metadata.h>

namespace atlasdb {
  namespace db {

    class catalog {
    public:

      auto es_cache() const {
        return _es_cache;
      }

      auto cluster_info() const {
        return _cluster_info;
      }

      auto datanode_info() const {
        return _datanode_info;
      }

      void es_cache(const es_cache& info) {
        _datanode_info = info;
      }

      void cluster_info(const cluster_info& info) {
        _datanode_info = info;
      }

      void datanode_info(const datanode_info& info) {
        _datanode_info = info;
      }

      void put(const es_info& info) {
        _es_cache[info.name] = info;
      }

      size_t es_count() const {
        return _es_cache.size();
      }

    private:

      cluster_info _cluster_info;
      datanode_info _datanode_info;

      std::map<std::string, es_info> _es_cache;
    };
  } // namespace db
} // namespace atlasdb

#endif /* DB_CATALOG_H_ */
