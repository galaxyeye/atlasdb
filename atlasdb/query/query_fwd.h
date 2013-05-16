/*
 * query_fwd.h
 *
 *  Created on: May 2, 2013
 *      Author: vincent
 */

#ifndef ATLASDB_QUERY_QUERY_FWD_H_
#define ATLASDB_QUERY_QUERY_FWD_H_

namespace atlasdb {
  namespace query {

    class node;
    class table;
    class logic_and;
    class logic_or;
    class operation;
    class ir_tree;
    class engine;

    typedef std::shared_ptr<engine> engine_ptr;

  } // query
} // atlasdb


#endif /* QUERY_FWD_H_ */
