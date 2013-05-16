/*
 * storage.cpp
 *
 *  Created on: May 3, 2013
 *      Author: vincent
 */

#include <atlasdb/storage/btree/btree.h>
#include <atlasdb/storage/btree/btree_map.h>

using namespace atlasdb::storage;

int main() {
  std::less<int> compare;
  std::allocator<int> alloc;

  btree<btree_map_params<int, int, std::less<int>, std::allocator<int>, 100>> b(compare, alloc);

  btree_map<int, int> m;
  btree_multimap<int, int> m2;

  return 0;
}
