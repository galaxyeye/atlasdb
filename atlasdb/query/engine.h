/*
 * engine.h
 *
 *  Created on: Apr 12, 2013
 *      Author: vincent
 */

#ifndef ATLASDB_ENGINE_QUERY_ENGINE_H_
#define ATLASDB_ENGINE_QUERY_ENGINE_H_

#include <string>
#include <memory>

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/graph_utility.hpp> // boost::make_list

#include <atlas/graph/tree.h>

#include <atlasdb/query/ir_builder.h>
#include <atlasdb/query/processor.h>

namespace atlasdb {
  namespace query {

    class engine {
    public:

      engine(const ptree& q) {
        ir_builder builder(q);
        _ir = builder();
      }

      engine(ptree&& q) {
        ir_builder builder(q);
        _ir = builder();
      }

    public:

      void execute() {
        using boost::depth_first_search;
        using boost::make_dfs_visitor;
        using boost::visitor;
        using boost::make_list;

        typedef atlas::tree_node_initializer<node, initializer_tree> initialize_visitor;
        typedef atlas::tree_node_discover<node, attacher_tree> attach_visitor;
        typedef atlas::tree_edge_examiner<node, executor_tree> execute_visitor;

        depth_first_search(_ir, visitor(make_dfs_visitor(
            make_list(initialize_visitor(nodes), attach_visitor(nodes), execute_visitor(nodes)))));
      }

      const char* result() {
        return nullptr;
      }

    private:

      ir_tree _ir;
    };

  } // query
} // atlasdb

#endif // QUERY_ENGINE_H_
