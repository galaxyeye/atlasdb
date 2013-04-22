/*
 * ir_tree.h
 *
 *  Created on: Apr 12, 2013
 *      Author: vincent
 */

#ifndef IR_TREE_H_
#define IR_TREE_H_

#include <boost/graph/adjacency_list.hpp>

namespace atlasdb {
  namespace query {

    enum { i_node, i_table, i_logic_and, i_logic_or, i_operation, n_node };

    node nodes[] = { node, table, logic_and, logic_or, operation };

    typedef boost::adjacency_list<boost::mapS, boost::vecS, boost::directedS> ir_tree;

  } // query
} // atlasdb

#endif /* IR_TREE_H_ */
