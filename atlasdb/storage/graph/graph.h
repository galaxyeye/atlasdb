/*
 * graph.h
 *
 *  Created on: Apr 21, 2013
 *      Author: vincent
 */

#ifndef atlasdb_STORAGE_GRAPH_H_
#define atlasdb_STORAGE_GRAPH_H_

#include <boost/graph/adjacency_list.hpp>

namespace atlasdb {
  namespace storage {

    typedef unsigned long long VertexID;

    typedef double EdgeWeight;

    typedef boost::adjacency_list
    <
      boost::vecS,
      boost::vecS,
      boost::bidirectionalS,
      VertexInfo,
      EdgeInfo
    > Graph;

    #define GRAPH_WEIGHTMAP(G) get(&EdgeInfo::weight, G)
    typedef boost::property_map<Graph, EdgeWeight EdgeInfo::*>::type weightmap_type;

    #define GRAPH_INDEXMAP(G)  get(vertex_index, G)
    typedef boost::property_map<Graph, vertex_index_t>::type indexmap_type;

    #define GRAPH_IDMAP(G)     get(&VertexInfo::id, G)
    typedef boost::property_map<Graph, VertexID VertexInfo::*>::type idmap_type;

    class cursor {};

    class row {};

    class graph {
      cursor *cursor;
      row row_info;

    public:

      enum error_code {
        OK = 0,
        NO_MORE_DATA,
        EDGE_NOT_FOUND,
        INVALID_WEIGHT,
        DUPLICATE_EDGE,
        CANNOT_ADD_VERTEX,
        CANNOT_ADD_EDGE,
        MISC_FAIL
      };

      struct current_row_st {};

      static inline current_row_st current_row() {
        return current_row_st();
      }

      unsigned vertices_count() const noexcept;
      unsigned edges_count() const noexcept;

      int delete_all(void) noexcept;

      int insert_edge(VertexID, VertexID, EdgeWeight, bool = 0) throw ();
      int modify_edge(VertexID, VertexID, EdgeWeight) throw ();
      int delete_edge(VertexID, VertexID) throw ();

      int modify_edge(current_row_st, VertexID*, VertexID*, EdgeWeight*, bool = 0) throw ();
      int delete_edge(current_row_st) throw ();

      int replace_edge(VertexID orig, VertexID dest, EdgeWeight weight) throw () {
        return insert_edge(orig, dest, weight, true);
      }

      int search(int*, VertexID*, VertexID*) throw ();
      int random(bool) throw ();

      int fetch_row(row&) throw ();
      int fetch_row(row&, const void*) throw ();
      void row_ref(void*) throw ();

      static oqgraph* create(oqgraph_share*) throw ();
      static oqgraph_share *create() throw ();

      static void free(oqgraph*) throw ();
      static void free(oqgraph_share*) throw ();

      static const size_t sizeof_ref;
    };

  } // storage
} // atlasdb

#endif /* atlasdb_STORAGE_GRAPH_H_ */
