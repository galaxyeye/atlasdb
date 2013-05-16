/*
 * storage.cpp
 *
 *  Created on: May 3, 2013
 *      Author: vincent
 */

#include <atlasdb/storage/basic_index.h>
#include <atlasdb/storage/basic_indexer.h>
#include <atlasdb/storage/basic_repository.h>
#include <atlasdb/storage/basic_storehouse.h>

#include <atlasdb/storage/bits/basic_indexer.tcc>
#include <atlasdb/storage/bits/basic_repository.tcc>
#include <atlasdb/storage/bits/basic_storehouse.tcc>

using namespace atlasdb::storage;

int main() {
  basic_index<int, int, int> index;

  basic_indexer<int, int, int> indexer;

  basic_repository<int, int, int> repository;

  basic_storehouse<int, int, int> storehouse("universe");

}
