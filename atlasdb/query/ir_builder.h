/*
 * translator.h
 *
 *  Created on: Apr 21, 2013
 *      Author: vincent
 */

#ifndef ATLASDB_QUERY_IR_BUILDER_H_
#define ATLASDB_QUERY_IR_BUILDER_H_

#include <boost/property_tree/ptree.hpp>

#include <atlasdb/query/query_fwd.h>

namespace atlasdb {
  namespace query {

    using std::string;
    using boost::property_tree::ptree;

    class ir_builder {
    public:

      ir_builder(const ptree& stree) {
      }

    public:

      ir_tree operator()(const ptree& stree) {
        ir_tree ir;
        return parse(stree, std::move(ir), new node);
      }

    protected:

      ir_tree parse(const ptree& stree, ir_tree&& ir, node*) {
        boost::add_edge(i_node, i_table, ir);

        return ir;
      }

      ir_tree parse(const ptree& stree, ir_tree&& ir, table*) {
        boost::add_edge(i_node, i_table, ir);

        return ir;
      }

    private:

      std::vector<node> _nodes = { node, table };
    };

  } // query
} // atlasdb

#endif /* TRANSLATOR_H_ */
