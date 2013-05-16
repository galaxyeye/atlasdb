/*
 * processor.h
 *
 *  Created on: Apr 14, 2013
 *      Author: vincent
 */

#ifndef ATLASDB_QUERY_IR_PROCESSOR_H_
#define ATLASDB_QUERY_IR_PROCESSOR_H_

#include <string>
#include <algorithm>

// #include <boost/mpl/at.hpp>
//#include <boost/fusion/sequence.hpp>
//#include <boost/fusion/container/map.hpp>
//#include <boost/fusion/algorithm.hpp>
//#include <boost/fusion/support/pair.hpp>

#include <atlas/mpl/at_path.h>
#include <atlas/graph/tree.h>

namespace atlasdb {
  namespace query {

    using std::string;

    // much more efficient but ugly
    template<typename ProcessorTree>
    struct efficient_ir_processor {

      typedef ProcessorTree processor_tree;

      efficient_ir_processor(node* current, node* parent) : current(current), parent(parent) {}

      void operator()(node* current, node* parent) {
        using mpl::at_path;

        auto parent_type = typeid(*parent);
        auto current_type = typeid(*current);

        if (parent_type == typeid(table)) {
          if (current_type == typeid(operation)) {
            at_path<processor_tree, table, operation>()();
          }
          else if (current_type == typeid(logic_and)) {
            at_path<processor_tree, table, logic_and>()();
          }
          else if (current_type == typeid(table)) {
            at_path<processor_tree, table, logic_or>()();
          }
        }
        else if (parent_type == typeid(logic_and)) {
          if (current_type == typeid(operation)) {
            at_path<processor_tree, logic_and, operation>()();
          }
          else if (current_type == typeid(logic_and)) {
            at_path<processor_tree, logic_and, logic_and>()();
          }
          else if (current_type == typeid(logic_or)) {
            at_path<processor_tree, logic_and, logic_or>()();
          }
        }
        else if (parent_type == typeid(logic_or)) {
          if (current_type == typeid(operation)) {
            at_path<processor_tree, logic_or, operation>()();
          }
          else if (current_type == typeid(logic_and)) {
            at_path<processor_tree, logic_or, logic_and>()();
          }
          else if (current_type == typeid(logic_or)) {
            at_path<processor_tree, logic_or, logic_or>()();
          }
        }
      }

      node* current;
      node* parent;
    };

    struct ir_initializer {

      void operator()(table& table) {
        table_name = table.name();
      }

      void operator()(operation& op) {
        indexes.push_back(op.name());
      }

      string table_name;
      std::vector<string> indexes;
    }; // attacher

    struct storage_attacher {

      storage_attacher(const string& table, const std::vector<string> indexes) : storehouse(table, indexes) {}

      void operator()(table& table) {
        table.repository(storehouse);
      }

      void operator()(operation& op) {
        op.index(storehouse);
      }

      storehouse_ptr storehouse;
    }; // attacher

    struct query_executor {

      void operator()(operation& op, table& t) {
        std::copy(op.index()->begin(), op.index()->end(), t.keyset().begin());
      }

      // TODO : optimization
      void operator()(operation& op, logic_and& l) {
        std::set<key> keyset;
        std::set_intersection(op.index()->begin(), op.index()->end(), l.keyset().begin(), l.keyset().end(), keyset.begin());
        l.keyset().swap(keyset);
      }

      // TODO : optimization
      void operator()(operation& op, logic_or& l) {
        std::set<key> keyset;
        std::set_union(op.index()->begin(), op.index()->end(), l.keyset().begin(), l.keyset().end(), keyset.begin());
        l.keyset().swap(keyset);
      }

    }; // query_executor

    namespace {

      typedef processor_wrapper<storage_attacher, table> ap;
      typedef processor_wrapper<storage_attacher, operation> ap2;

      typedef processor_wrapper<query_executor, operation, table> ep;
      typedef processor_wrapper<query_executor, operation, logic_and> ep2;
      typedef processor_wrapper<query_executor, operation, logic_or> ep3;

      using boost::fusion::map;
      using boost::fusion::pair;
      using std::nullptr_t;

      typedef map
          <
            pair<node, map<table, nullptr_t>>,
            pair<table, map<pair<logic_and, nullptr_t>, pair<logic_or, nullptr_t>, pair<operation, nullptr_t>>>,
            pair<logic_and, map<pair<logic_and, nullptr_t>, pair<logic_or, nullptr_t>, pair<operation, nullptr_t>>>,
            pair<logic_or, map<pair<logic_and, nullptr_t>, pair<logic_or, nullptr_t>, pair<operation, nullptr_t>>>
          > __initializer_tree;

      typedef map
          <
            pair<node, map<table, ap>>,
            pair<table, map<pair<logic_and, nullptr_t>, pair<logic_or, nullptr_t>, pair<operation, ap2>>>,
            pair<logic_and, map<pair<logic_and, nullptr_t>, pair<logic_or, nullptr_t>, pair<operation, ap2>>>,
            pair<logic_or, map<pair<logic_and, nullptr_t>, pair<logic_or, nullptr_t>, pair<operation, ap2>>>
          > __attacher_tree;

      typedef map
          <
            pair<node, map<table, nullptr_t>>,
            pair<table, map<pair<logic_and, nullptr_t>, pair<logic_or, nullptr_t>, pair<operation, ep>>>,
            pair<logic_and, map<pair<logic_and, nullptr_t>, pair<logic_or, nullptr_t>, pair<operation, ep2>>>,
            pair<logic_or, map<pair<logic_and, nullptr_t>, pair<logic_or, nullptr_t>, pair<operation, ep3>>>
          > __executor_tree;
    }

    typedef __initializer_tree initializer_tree;
    typedef __attacher_tree attacher_tree;
    typedef __executor_tree executor_tree;

  } // query
} // atlasdb

#endif /* ATLASDB_QUERY_IR_PROCESSOR_H_ */
