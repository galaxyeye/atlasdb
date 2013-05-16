// Copyright 2013 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// A btree implementation of the STL set and map interfaces. A btree is both
// smaller and faster than STL set/map. The red-black tree implementation of
// STL set/map has an overhead of 3 pointers (left, right and parent) plus the
// node color information for each stored value. So a set<int32> consumes 20
// bytes for each value stored. This btree implementation stores multiple
// values on fixed size nodes (usually 256 bytes) and doesn't store child
// pointers for leaf nodes. The result is that a btree_set<int32> may use much
// less memory per stored value. For the random insertion benchmark in
// btree_test.cc, a btree_set<int32> with node-size of 256 uses 4.9 bytes per
// stored value.
//
// The packing of multiple values on to each node of a btree has another effect
// besides better space utilization: better cache locality due to fewer cache
// lines being accessed. Better cache locality translates into faster
// operations.
//
// CAVEATS
//
// Insertions and deletions on a btree can cause splitting, merging or
// rebalancing of btree nodes. And even without these operations, insertions
// and deletions on a btree will move values around within a node. In both
// cases, the result is that insertions and deletions can invalidate iterators
// pointing to values other than the one being inserted/deleted. This is
// notably different from STL set/map which takes care to not invalidate
// iterators on insert/erase except, of course, for iterators pointing to the
// value being erased.  A partial workaround when erasing is available:
// erase() returns an iterator pointing to the item just after the one that was
// erased (or end() if none exists).  See also safe_btree.

#ifndef ATLASDB_STORAGE_BTREE_BTREE_TCC_
#define ATLASDB_STORAGE_BTREE_BTREE_TCC_

#include <cassert>
#include <string>
#include <functional>
#include <type_traits>
#include <ostream>

namespace atlasdb {
  namespace storage {

    ////
    // btree_node methods
    template<typename P>
    inline void btree_node<P>::insert_value(int i, const value_type &x) {
      assert(i <= count());
      value_init(count(), x);
      for (int j = count(); j > i; --j) {
        value_swap(j, this, j - 1);
      }
      set_count(count() + 1);

      if (!leaf()) {
        ++i;
        for (int j = count(); j > i; --j) {
          *mutable_child(j) = child(j - 1);
          child(j)->set_position(j);
        }
        *mutable_child(i) = nullptr;
      }
    }

    template<typename P>
    inline void btree_node<P>::remove_value(int i) {
      if (!leaf()) {
        assert(child(i + 1)->count() == 0);
        for (int j = i + 1; j < count(); ++j) {
          *mutable_child(j) = child(j + 1);
          child(j)->set_position(j);
        }
        *mutable_child(count()) = nullptr;
      }

      set_count(count() - 1);
      for (; i < count(); ++i) {
        value_swap(i, this, i + 1);
      }
      value_destroy(i);
    }

    template<typename P>
    void btree_node<P>::rebalance_right_to_left(btree_node *src, int to_move) {
      assert(parent() == src->parent());
      assert(position() + 1 == src->position());
      assert(src->count() >= count());
      assert(to_move >= 1);
      assert(to_move <= src->count());

      // Make room in the left node for the new values.
      for (int i = 0; i < to_move; ++i) {
        value_init(i + count());
      }

      // Move the delimiting value to the left node and the new delimiting value
      // from the right node.
      value_swap(count(), parent(), position());
      parent()->value_swap(position(), src, to_move - 1);

      // Move the values from the right to the left node.
      for (int i = 1; i < to_move; ++i) {
        value_swap(count() + i, src, i - 1);
      }

      // Shift the values in the right node to their correct position.
      for (int i = to_move; i < src->count(); ++i) {
        src->value_swap(i - to_move, src, i);
      }

      for (int i = 1; i <= to_move; ++i) {
        src->value_destroy(src->count() - i);
      }

      if (!leaf()) {
        // Move the child pointers from the right to the left node.
        for (int i = 0; i < to_move; ++i) {
          set_child(1 + count() + i, src->child(i));
        }
        for (int i = 0; i <= src->count() - to_move; ++i) {
          assert(i + to_move <= src->max_count());
          src->set_child(i, src->child(i + to_move));
          *src->mutable_child(i + to_move) = nullptr;
        }
      }

      // Fixup the counts on the src and dest nodes.
      set_count(count() + to_move);
      src->set_count(src->count() - to_move);
    }

    template<typename P>
    void btree_node<P>::rebalance_left_to_right(btree_node *dest, int to_move) {
      assert(parent() == dest->parent());
      assert(position() + 1 == dest->position());
      assert(count() >= dest->count());
      assert(to_move >= 1);
      assert(to_move <= count());

      // Make room in the right node for the new values.
      for (int i = 0; i < to_move; ++i) {
        dest->value_init(i + dest->count());
      }
      for (int i = dest->count() - 1; i >= 0; --i) {
        dest->value_swap(i, dest, i + to_move);
      }

      // Move the delimiting value to the right node and the new delimiting value
      // from the left node.
      dest->value_swap(to_move - 1, parent(), position());
      parent()->value_swap(position(), this, count() - to_move);
      value_destroy(count() - to_move);

      // Move the values from the left to the right node.
      for (int i = 1; i < to_move; ++i) {
        value_swap(count() - to_move + i, dest, i - 1);
        value_destroy(count() - to_move + i);
      }

      if (!leaf()) {
        // Move the child pointers from the left to the right node.
        for (int i = dest->count(); i >= 0; --i) {
          dest->set_child(i + to_move, dest->child(i));
          *dest->mutable_child(i) = nullptr;
        }
        for (int i = 1; i <= to_move; ++i) {
          dest->set_child(i - 1, child(count() - to_move + i));
          *mutable_child(count() - to_move + i) = nullptr;
        }
      }

      // Fixup the counts on the src and dest nodes.
      set_count(count() - to_move);
      dest->set_count(dest->count() + to_move);
    }

    template<typename P>
    void btree_node<P>::split(btree_node *dest, int insert_position) {
      assert(dest->count() == 0);

      // We bias the split based on the position being inserted. If we're
      // inserting at the beginning of the left node then bias the split to put
      // more values on the right node. If we're inserting at the end of the
      // right node then bias the split to put more values on the left node.
      if (insert_position == 0) {
        dest->set_count(count() - 1);
      }
      else if (insert_position == max_count()) {
        dest->set_count(0);
      }
      else {
        dest->set_count(count() / 2);
      }
      set_count(count() - dest->count());
      assert(count() >= 1);

      // Move values from the left sibling to the right sibling.
      for (int i = 0; i < dest->count(); ++i) {
        dest->value_init(i);
        value_swap(count() + i, dest, i);
        value_destroy(count() + i);
      }

      // The split key is the largest value in the left sibling.
      set_count(count() - 1);
      parent()->insert_value(position(), value_type());
      value_swap(count(), parent(), position());
      value_destroy(count());
      parent()->set_child(position() + 1, dest);

      if (!leaf()) {
        for (int i = 0; i <= dest->count(); ++i) {
          assert(child(count() + i + 1) != nullptr);
          dest->set_child(i, child(count() + i + 1));
          *mutable_child(count() + i + 1) = nullptr;
        }
      }
    }

    template<typename P>
    void btree_node<P>::merge(btree_node *src) {
      assert(parent() == src->parent());
      assert(position() + 1 == src->position());

      // Move the delimiting value to the left node.
      value_init(count());
      value_swap(count(), parent(), position());

      // Move the values from the right to the left node.
      for (int i = 0; i < src->count(); ++i) {
        value_init(1 + count() + i);
        value_swap(1 + count() + i, src, i);
        src->value_destroy(i);
      }

      if (!leaf()) {
        // Move the child pointers from the right to the left node.
        for (int i = 0; i <= src->count(); ++i) {
          set_child(1 + count() + i, src->child(i));
          *src->mutable_child(i) = nullptr;
        }
      }

      // Fixup the counts on the src and dest nodes.
      set_count(1 + count() + src->count());
      src->set_count(0);

      // Remove the value on the parent node.
      parent()->remove_value(position());
    }

    template<typename P>
    void btree_node<P>::swap(btree_node *x) {
      assert(leaf() == x->leaf());

      // Swap the values.
      for (int i = count(); i < x->count(); ++i) {
        value_init(i);
      }
      for (int i = x->count(); i < count(); ++i) {
        x->value_init(i);
      }
      int n = std::max(count(), x->count());
      for (int i = 0; i < n; ++i) {
        value_swap(i, x, i);
      }
      for (int i = count(); i < x->count(); ++i) {
        x->value_destroy(i);
      }
      for (int i = x->count(); i < count(); ++i) {
        value_destroy(i);
      }

      if (!leaf()) {
        // Swap the child pointers.
        for (int i = 0; i <= n; ++i) {
          btree_swap_helper(*mutable_child(i), *x->mutable_child(i));
        }
        for (int i = 0; i <= count(); ++i) {
          x->child(i)->fields_.parent = x;
        }
        for (int i = 0; i <= x->count(); ++i) {
          child(i)->fields_.parent = this;
        }
      }

      // Swap the counts.
      btree_swap_helper(fields_.count, x->fields_.count);
    }

  ////
  // btree_iterator methods
    template<typename N, typename R, typename P>
    void btree_iterator<N, R, P>::increment_slow() {
      if (node->leaf()) {
        assert(position >= node->count());
        self_type save(*this);
        while (position == node->count() && !node->is_root()) {
          assert(node->parent()->child(node->position()) == node);
          position = node->position();
          node = node->parent();
        }
        if (position == node->count()) {
          *this = save;
        }
      }
      else {
        assert(position < node->count());
        node = node->child(position + 1);
        while (!node->leaf()) {
          node = node->child(0);
        }
        position = 0;
      }
    }

    template<typename N, typename R, typename P>
    void btree_iterator<N, R, P>::increment_by(int count) {
      while (count > 0) {
        if (node->leaf()) {
          int rest = node->count() - position;
          position += std::min(rest, count);
          count = count - rest;
          if (position < node->count()) {
            return;
          }
        }
        else {
          --count;
        }
        increment_slow();
      }
    }

    template<typename N, typename R, typename P>
    void btree_iterator<N, R, P>::decrement_slow() {
      if (node->leaf()) {
        assert(position <= -1);
        self_type save(*this);
        while (position < 0 && !node->is_root()) {
          assert(node->parent()->child(node->position()) == node);
          position = node->position() - 1;
          node = node->parent();
        }
        if (position < 0) {
          *this = save;
        }
      }
      else {
        assert(position >= 0);
        node = node->child(position);
        while (!node->leaf()) {
          node = node->child(node->count());
        }
        position = node->count() - 1;
      }
    }

    ////
    // btree methods
      template<typename P>
      btree<P>::btree(const key_compare &comp, const allocator_type &alloc) :
          key_compare(comp), root_(alloc, nullptr) {
      }

      template<typename P>
      btree<P>::btree(const self_type &x) :
          key_compare(x.key_comp()), root_(x.internal_allocator(), nullptr) {
        assign(x);
      }

      template<typename P> template<typename ValuePointer>
      std::pair<typename btree<P>::iterator, bool> btree<P>::insert_unique(const key_type &key, ValuePointer value) {
        if (empty()) {
          *mutable_root() = new_leaf_root_node(1);
        }

        std::pair<iterator, int> res = internal_locate(key, iterator(root(), 0));
        iterator &iter = res.first;
        if (res.second == kExactMatch) {
          // The key already exists in the tree, do nothing.
          return std::make_pair(internal_last(iter), false);
        }
        else if (!res.second) {
          iterator last = internal_last(iter);
          if (last.node && !compare_keys(key, last.key())) {
            // The key already exists in the tree, do nothing.
            return std::make_pair(last, false);
          }
        }

        return std::make_pair(internal_insert(iter, *value), true);
      }

      template<typename P>
      inline typename btree<P>::iterator btree<P>::insert_unique(iterator position, const value_type &v) {
        if (!empty()) {
          const key_type &key = params_type::key(v);
          if (position == end() || compare_keys(key, position.key())) {
            iterator prev = position;
            if (position == begin() || compare_keys((--prev).key(), key)) {
              // prev.key() < key < position.key()
              return internal_insert(position, v);
            }
          }
          else if (compare_keys(position.key(), key)) {
            iterator next = position;
            ++next;
            if (next == end() || compare_keys(key, next.key())) {
              // position.key() < key < next.key()
              return internal_insert(next, v);
            }
          }
          else {
            // position.key() == key
            return position;
          }
        }
        return insert_unique(v).first;
      }

      template<typename P> template<typename InputIterator>
      void btree<P>::insert_unique(InputIterator b, InputIterator e) {
        for (; b != e; ++b) {
          insert_unique(end(), *b);
        }
      }

      template<typename P> template<typename ValuePointer>
      typename btree<P>::iterator btree<P>::insert_multi(const key_type &key, ValuePointer value) {
        if (empty()) {
          *mutable_root() = new_leaf_root_node(1);
        }

        iterator iter = internal_upper_bound(key, iterator(root(), 0));
        if (!iter.node) {
          iter = end();
        }
        return internal_insert(iter, *value);
      }

      template<typename P>
      typename btree<P>::iterator btree<P>::insert_multi(iterator position, const value_type &v) {
        if (!empty()) {
          const key_type &key = params_type::key(v);
          if (position == end() || !compare_keys(position.key(), key)) {
            iterator prev = position;
            if (position == begin() || !compare_keys(key, (--prev).key())) {
              // prev.key() <= key <= position.key()
              return internal_insert(position, v);
            }
          }
          else {
            iterator next = position;
            ++next;
            if (next == end() || !compare_keys(next.key(), key)) {
              // position.key() < key <= next.key()
              return internal_insert(next, v);
            }
          }
        }
        return insert_multi(v);
      }

      template<typename P> template<typename InputIterator>
      void btree<P>::insert_multi(InputIterator b, InputIterator e) {
        for (; b != e; ++b) {
          insert_multi(end(), *b);
        }
      }

      template<typename P>
      void btree<P>::assign(const self_type &x) {
        clear();

        *mutable_key_comp() = x.key_comp();
        *mutable_internal_allocator() = x.internal_allocator();

        // Assignment can avoid key comparisons because we know the order of the
        // values is the same order we'll store them in.
        for (const_iterator iter = x.begin(); iter != x.end(); ++iter) {
          if (empty()) {
            insert_multi(*iter);
          }
          else {
            // If the btree is not empty, we can just insert the new value at the end
            // of the tree!
            internal_insert(end(), *iter);
          }
        }
      }

      template<typename P>
      typename btree<P>::iterator btree<P>::erase(iterator iter) {
        bool internal_delete = false;
        if (!iter.node->leaf()) {
          // Deletion of a value on an internal node. Swap the key with the largest
          // value of our left child. This is easy, we just decrement iter.
          iterator tmp_iter(iter--);
          assert(iter.node->leaf());
          assert(!compare_keys(tmp_iter.key(), iter.key()));
          iter.node->value_swap(iter.position, tmp_iter.node, tmp_iter.position);
          internal_delete = true;
          --*mutable_size();
        }
        else if (!root()->leaf()) {
          --*mutable_size();
        }

        // Delete the key from the leaf.
        iter.node->remove_value(iter.position);

        // We want to return the next value after the one we just erased. If we
        // erased from an internal node (internal_delete == true), then the next
        // value is ++(++iter). If we erased from a leaf node (internal_delete ==
        // false) then the next value is ++iter. Note that ++iter may point to an
        // internal node and the value in the internal node may move to a leaf node
        // (iter.node) when rebalancing is performed at the leaf level.

        // Merge/rebalance as we walk back up the tree.
        iterator res(iter);
        for (;;) {
          if (iter.node == root()) {
            try_shrink();
            if (empty()) {
              return end();
            }
            break;
          }
          if (iter.node->count() >= kMinNodeValues) {
            break;
          }
          bool merged = try_merge_or_rebalance(&iter);
          if (iter.node->leaf()) {
            res = iter;
          }
          if (!merged) {
            break;
          }
          iter.node = iter.node->parent();
        }

        // Adjust our return value. If we're pointing at the end of a node, advance
        // the iterator.
        if (res.position == res.node->count()) {
          res.position = res.node->count() - 1;
          ++res;
        }
        // If we erased from an internal node, advance the iterator.
        if (internal_delete) {
          ++res;
        }
        return res;
      }

      template<typename P>
      int btree<P>::erase(iterator begin, iterator end) {
        int count = distance(begin, end);
        for (int i = 0; i < count; i++) {
          begin = erase(begin);
        }
        return count;
      }

      template<typename P>
      int btree<P>::erase_unique(const key_type &key) {
        iterator iter = internal_find_unique(key, iterator(root(), 0));
        if (!iter.node) {
          // The key doesn't exist in the tree, return nothing done.
          return 0;
        }
        erase(iter);
        return 1;
      }

      template<typename P>
      int btree<P>::erase_multi(const key_type &key) {
        iterator begin = internal_lower_bound(key, iterator(root(), 0));
        if (!begin.node) {
          // The key doesn't exist in the tree, return nothing done.
          return 0;
        }
        // Delete all of the keys between begin and upper_bound(key).
        iterator end = internal_end(internal_upper_bound(key, iterator(root(), 0)));
        return erase(begin, end);
      }

      template<typename P>
      void btree<P>::clear() {
        if (root() != nullptr) {
          internal_clear(root());
        }
        *mutable_root() = nullptr;
      }

      template<typename P>
      void btree<P>::swap(self_type &x) {
        std::swap(static_cast<key_compare&>(*this), static_cast<key_compare&>(x));
        std::swap(root_, x.root_);
      }

      template<typename P>
      void btree<P>::verify() const {
        if (root() != nullptr) {
          assert(size() == internal_verify(root(), nullptr, nullptr));
          assert(leftmost() == (++const_iterator(root(), -1)).node);
          assert(rightmost() == (--const_iterator(root(), root()->count())).node);
          assert(leftmost()->leaf());
          assert(rightmost()->leaf());
        }
        else {
          assert(size() == 0);
          assert(leftmost() == nullptr);
          assert(rightmost() == nullptr);
        }
      }

      template<typename P>
      void btree<P>::rebalance_or_split(iterator *iter) {
        node_type *&node = iter->node;
        int &insert_position = iter->position;
        assert(node->count() == node->max_count());

        // First try to make room on the node by rebalancing.
        node_type *parent = node->parent();
        if (node != root()) {
          if (node->position() > 0) {
            // Try rebalancing with our left sibling.
            node_type *left = parent->child(node->position() - 1);
            if (left->count() < left->max_count()) {
              // We bias rebalancing based on the position being inserted. If we're
              // inserting at the end of the right node then we bias rebalancing to
              // fill up the left node.
              int to_move = (left->max_count() - left->count()) / (1 + (insert_position < left->max_count()));
              to_move = std::max(1, to_move);

              if (((insert_position - to_move) >= 0) || ((left->count() + to_move) < left->max_count())) {
                left->rebalance_right_to_left(node, to_move);

                assert(node->max_count() - node->count() == to_move);
                insert_position = insert_position - to_move;
                if (insert_position < 0) {
                  insert_position = insert_position + left->count() + 1;
                  node = left;
                }

                assert(node->count() < node->max_count());
                return;
              }
            }
          }

          if (node->position() < parent->count()) {
            // Try rebalancing with our right sibling.
            node_type *right = parent->child(node->position() + 1);
            if (right->count() < right->max_count()) {
              // We bias rebalancing based on the position being inserted. If we're
              // inserting at the beginning of the left node then we bias rebalancing
              // to fill up the right node.
              int to_move = (right->max_count() - right->count()) / (1 + (insert_position > 0));
              to_move = std::max(1, to_move);

              if ((insert_position <= (node->count() - to_move)) || ((right->count() + to_move) < right->max_count())) {
                node->rebalance_left_to_right(right, to_move);

                if (insert_position > node->count()) {
                  insert_position = insert_position - node->count() - 1;
                  node = right;
                }

                assert(node->count() < node->max_count());
                return;
              }
            }
          }

          // Rebalancing failed, make sure there is room on the parent node for a new
          // value.
          if (parent->count() == parent->max_count()) {
            iterator parent_iter(node->parent(), node->position());
            rebalance_or_split(&parent_iter);
          }
        }
        else {
          // Rebalancing not possible because this is the root node.
          if (root()->leaf()) {
            // The root node is currently a leaf node: create a new root node and set
            // the current root node as the child of the new root.
            parent = new_internal_root_node();
            parent->set_child(0, root());
            *mutable_root() = parent;
            assert(*mutable_rightmost() == parent->child(0));
          }
          else {
            // The root node is an internal node. We do not want to create a new root
            // node because the root node is special and holds the size of the tree
            // and a pointer to the rightmost node. So we create a new internal node
            // and move all of the items on the current root into the new node.
            parent = new_internal_node(parent);
            parent->set_child(0, parent);
            parent->swap(root());
            node = parent;
          }
        }

        // Split the node.
        node_type *split_node;
        if (node->leaf()) {
          split_node = new_leaf_node(parent);
          node->split(split_node, insert_position);
          if (rightmost() == node) {
            *mutable_rightmost() = split_node;
          }
        }
        else {
          split_node = new_internal_node(parent);
          node->split(split_node, insert_position);
        }

        if (insert_position > node->count()) {
          insert_position = insert_position - node->count() - 1;
          node = split_node;
        }
      }

      template<typename P>
      void btree<P>::merge_nodes(node_type *left, node_type *right) {
        left->merge(right);
        if (right->leaf()) {
          if (rightmost() == right) {
            *mutable_rightmost() = left;
          }
          delete_leaf_node(right);
        }
        else {
          delete_internal_node(right);
        }
      }

      template<typename P>
      bool btree<P>::try_merge_or_rebalance(iterator *iter) {
        node_type *parent = iter->node->parent();
        if (iter->node->position() > 0) {
          // Try merging with our left sibling.
          node_type *left = parent->child(iter->node->position() - 1);
          if ((1 + left->count() + iter->node->count()) <= left->max_count()) {
            iter->position += 1 + left->count();
            merge_nodes(left, iter->node);
            iter->node = left;
            return true;
          }
        }
        if (iter->node->position() < parent->count()) {
          // Try merging with our right sibling.
          node_type *right = parent->child(iter->node->position() + 1);
          if ((1 + iter->node->count() + right->count()) <= right->max_count()) {
            merge_nodes(iter->node, right);
            return true;
          }
          // Try rebalancing with our right sibling. We don't perform rebalancing if
          // we deleted the first element from iter->node and the node is not
          // empty. This is a small optimization for the common pattern of deleting
          // from the front of the tree.
          if ((right->count() > kMinNodeValues) && ((iter->node->count() == 0) || (iter->position > 0))) {
            int to_move = (right->count() - iter->node->count()) / 2;
            to_move = std::min(to_move, right->count() - 1);
            iter->node->rebalance_right_to_left(right, to_move);
            return false;
          }
        }
        if (iter->node->position() > 0) {
          // Try rebalancing with our left sibling. We don't perform rebalancing if
          // we deleted the last element from iter->node and the node is not
          // empty. This is a small optimization for the common pattern of deleting
          // from the back of the tree.
          node_type *left = parent->child(iter->node->position() - 1);
          if ((left->count() > kMinNodeValues) && ((iter->node->count() == 0) || (iter->position < iter->node->count()))) {
            int to_move = (left->count() - iter->node->count()) / 2;
            to_move = std::min(to_move, left->count() - 1);
            left->rebalance_left_to_right(iter->node, to_move);
            iter->position += to_move;
            return false;
          }
        }
        return false;
      }

      template<typename P>
      void btree<P>::try_shrink() {
        if (root()->count() > 0) {
          return;
        }
        // Deleted the last item on the root node, shrink the height of the tree.
        if (root()->leaf()) {
          assert(size() == 0);
          delete_leaf_node(root());
          *mutable_root() = nullptr;
        }
        else {
          node_type *child = root()->child(0);
          if (child->leaf()) {
            // The child is a leaf node so simply make it the root node in the tree.
            child->make_root();
            delete_internal_root_node();
            *mutable_root() = child;
          }
          else {
            // The child is an internal node. We want to keep the existing root node
            // so we move all of the values from the child node into the existing
            // (empty) root node.
            child->swap(root());
            delete_internal_node(child);
          }
        }
      }

      template<typename P> template<typename IterType>
      inline IterType btree<P>::internal_last(IterType iter) {
        while (iter.node && iter.position == iter.node->count()) {
          iter.position = iter.node->position();
          iter.node = iter.node->parent();
          if (iter.node->leaf()) {
            iter.node = nullptr;
          }
        }
        return iter;
      }

      template<typename P>
      inline typename btree<P>::iterator btree<P>::internal_insert(iterator iter, const value_type &v) {
        if (!iter.node->leaf()) {
          // We can't insert on an internal node. Instead, we'll insert after the
          // previous value which is guaranteed to be on a leaf node.
          --iter;
          ++iter.position;
        }
        if (iter.node->count() == iter.node->max_count()) {
          // Make room in the leaf for the new item.
          if (iter.node->max_count() < kNodeValues) {
            // Insertion into the root where the root is smaller that the full node
            // size. Simply grow the size of the root node.
            assert(iter.node == root());
            iter.node = new_leaf_root_node(std::min<int>(kNodeValues, 2 * iter.node->max_count()));
            iter.node->swap(root());
            delete_leaf_node(root());
            *mutable_root() = iter.node;
          }
          else {
            rebalance_or_split(&iter);
            ++*mutable_size();
          }
        }
        else if (!root()->leaf()) {
          ++*mutable_size();
        }
        iter.node->insert_value(iter.position, v);
        return iter;
      }

      template<typename P> template<typename IterType>
      inline std::pair<IterType, int> btree<P>::internal_locate(const key_type &key, IterType iter) const {
        return internal_locate_type::dispatch(key, *this, iter);
      }

      template<typename P> template<typename IterType>
      inline std::pair<IterType, int> btree<P>::internal_locate_plain_compare(const key_type &key, IterType iter) const {
        for (;;) {
          iter.position = iter.node->lower_bound(key, key_comp());
          if (iter.node->leaf()) {
            break;
          }
          iter.node = iter.node->child(iter.position);
        }
        return std::make_pair(iter, 0);
      }

      template<typename P> template<typename IterType>
      inline std::pair<IterType, int> btree<P>::internal_locate_compare_to(const key_type &key, IterType iter) const {
        for (;;) {
          int res = iter.node->lower_bound(key, key_comp());
          iter.position = res & kMatchMask;
          if (res & kExactMatch) {
            return std::make_pair(iter, static_cast<int>(kExactMatch));
          }
          if (iter.node->leaf()) {
            break;
          }
          iter.node = iter.node->child(iter.position);
        }
        return std::make_pair(iter, -kExactMatch);
      }

      template<typename P> template<typename IterType>
      IterType btree<P>::internal_lower_bound(const key_type &key, IterType iter) const {
        if (iter.node) {
          for (;;) {
            iter.position = iter.node->lower_bound(key, key_comp()) & kMatchMask;
            if (iter.node->leaf()) {
              break;
            }
            iter.node = iter.node->child(iter.position);
          }
          iter = internal_last(iter);
        }
        return iter;
      }

      template<typename P> template<typename IterType>
      IterType btree<P>::internal_upper_bound(const key_type &key, IterType iter) const {
        if (iter.node) {
          for (;;) {
            iter.position = iter.node->upper_bound(key, key_comp());
            if (iter.node->leaf()) {
              break;
            }
            iter.node = iter.node->child(iter.position);
          }
          iter = internal_last(iter);
        }
        return iter;
      }

      template<typename P> template<typename IterType>
      IterType btree<P>::internal_find_unique(const key_type &key, IterType iter) const {
        if (iter.node) {
          std::pair<IterType, int> res = internal_locate(key, iter);
          if (res.second == kExactMatch) {
            return res.first;
          }
          if (!res.second) {
            iter = internal_last(res.first);
            if (iter.node && !compare_keys(key, iter.key())) {
              return iter;
            }
          }
        }
        return IterType(nullptr, 0);
      }

      template<typename P> template<typename IterType>
      IterType btree<P>::internal_find_multi(const key_type &key, IterType iter) const {
        if (iter.node) {
          iter = internal_lower_bound(key, iter);
          if (iter.node) {
            iter = internal_last(iter);
            if (iter.node && !compare_keys(key, iter.key())) {
              return iter;
            }
          }
        }
        return IterType(nullptr, 0);
      }

      template<typename P>
      void btree<P>::internal_clear(node_type *node) {
        if (!node->leaf()) {
          for (int i = 0; i <= node->count(); ++i) {
            internal_clear(node->child(i));
          }
          if (node == root()) {
            delete_internal_root_node();
          }
          else {
            delete_internal_node(node);
          }
        }
        else {
          delete_leaf_node(node);
        }
      }

      template<typename P>
      void btree<P>::internal_dump(std::ostream &os, const node_type *node, int level) const {
        for (int i = 0; i < node->count(); ++i) {
          if (!node->leaf()) {
            internal_dump(os, node->child(i), level + 1);
          }
          for (int j = 0; j < level; ++j) {
            os << "  ";
          }
          os << node->key(i) << " [" << level << "]\n";
        }
        if (!node->leaf()) {
          internal_dump(os, node->child(node->count()), level + 1);
        }
      }

      template<typename P>
      int btree<P>::internal_verify(const node_type *node, const key_type *lo, const key_type *hi) const {
        assert(node->count() > 0);
        assert(node->count() <= node->max_count());
        if (lo) {
          assert(!compare_keys(node->key(0), *lo));
        }
        if (hi) {
          assert(!compare_keys(*hi, node->key(node->count() - 1)));
        }
        for (int i = 1; i < node->count(); ++i) {
          assert(!compare_keys(node->key(i), node->key(i - 1)));
        }
        int count = node->count();
        if (!node->leaf()) {
          for (int i = 0; i <= node->count(); ++i) {
            assert(node->child(i) != nullptr);
            assert(node->child(i)->parent() == node);
            assert(node->child(i)->position() == i);
            count += internal_verify(node->child(i), (i == 0) ? lo : &node->key(i - 1),
                (i == node->count()) ? hi : &node->key(i));
          }
        }
        return count;
      }

  } // storage
} // btree

#endif  // UTIL_BTREE_BTREE_H__
