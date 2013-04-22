/**
 *  ir.h
 *
 *  Created on: Oct 9, 2011
 *      Author: vincent
 */

#ifndef DB_METADATA_H_
#define DB_METADATA_H_

#include <string>
#include <vector>
#include <map>

namespace atlasdb {
  namespace db {

    struct cluster_info {
      std::vector<std::string> _catalog_ips;
      std::vector<std::string> _data_node_ips;
    };

    struct datanode_info {
      int es_count; // table count
      std::pair<std::string, std::string> public_endpoint; // <ip, port> for public access
      std::pair<std::string, std::string> internal_endpoint; // <ip, port> for in-cluster access
    };

    struct index_calc_info {
      int func; // 0 == JS_CODE, 1 == BY_FIELD
      std::string field_index;
      std::string data;
    };

    struct index_info {
      int type; // 0 == NORMAL, 1 == HASH_INDEX
      int key_type; // type from CSON module
      index_calc_info calc;
    };

    struct es_info {
      std::string name;
      int key_type; // type from CSON module
      std::map<std::string, index_info> index_infoes;
    };
  } // namespace db
} // namespace atlasdb

#endif // DB_METADATA_H_
