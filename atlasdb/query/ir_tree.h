/*
 * ir_tree.h
 *
 *  Created on: Apr 12, 2013
 *      Author: vincent
 */

#ifndef ATLASDB_QUERY_IR_TREE_H_
#define ATLASDB_QUERY_IR_TREE_H_

#include <boost/graph/adjacency_list.hpp>

#include <atlasdb/query/query_fwd.h>

namespace atlasdb {
  namespace query {

    enum { i_node, i_table, i_logic_and, i_logic_or, i_operation, n_node };

    node nodes[] = { node, table, logic_and, logic_or, operation };

    typedef boost::adjacency_list<boost::mapS, boost::vecS> ir_tree;

  } // query
} // atlasdb

#endif /* ATLASDB_QUERY_IR_TREE_H_ */
