/*
 *  ir.h
 *
 *  Created on: Oct 6, 2011
 *      Author: vincent
 */

/*
 *  Intermediate representation, which is a tree on which optimizations and physical
 *  queries performs.
 */

#ifndef ATLASDB_QUERY_IR_H_
#define ATLASDB_QUERY_IR_H_

#include <set>
#include <string>
#include <algorithm>

#include <atlasdb/query/storage.h>

namespace atlasdb {
  namespace query {

    using std::string;

    enum class op_t {
      equal_to,
      not_equal_to,
      greater,
      greater_equal,
      less,
      less_equal,
      begin_with,
      contain,
      end_with,
      in,
      not_in,
      between,
      root,
      not_root,
      function
    };

    class node {
    public:

      explicit node() {}

      virtual ~node() {}

    private:
    };

    class table : public node {
    public:

      table() {}

      virtual ~table() { }

      void name(const string& name) { _name = name; }

      const string& name() const { return _name; }

      std::set<key>& keyset() { return _keyset; }

      const std::set<key>& keyset() const { return _keyset; }

      void repository(repository_ptr repo) { _repository = repo; }

      repository_ptr& repository() const { return _repository; }

    private:

      string _name;
      std::set<key> _keyset;
      repository _repository;
    };

    class logic_and : public node {
    public:

      logic_and() {}

      virtual ~logic_and() { }

      std::set<key>& keyset() { return _keyset; }

      const std::set<key>& keyset() const { return _keyset; }

    private:

      std::set<key> _keyset;
    };

    class logic_or : public node {
    public:

      logic_or() { }

      virtual ~logic_or() { }

      const std::set<key>& keyset() const { return _keyset; }

      std::set<key>& keyset() { return _keyset; }

    private:

      std::set<key> _keyset;
    };

    class operation : public node {
    public:

      operation(op_t operation = op_t::equal_to) : _op(operation) {}

      virtual ~operation() {}

      void name(const string& name) { _name = name; }

      const string& name() const { return _name; }

      void op2set(op_t op) { _op = op; }

      void keyset(std::set<key>&& keyset) {}

      void index(const index_ptr& index) { _index = index; }

      index_ptr& index() { return _index; }

      const index_ptr& index() const { return _index; }

    protected:

      op_t _op = op_t::equal_to;
      string _name;
      index_ptr _index;
    };

  } // query
} // atlasdb

#endif // QUERY_IR_H_
