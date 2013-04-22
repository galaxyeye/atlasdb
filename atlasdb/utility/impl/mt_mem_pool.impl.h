/*
 * mt_mem_pool.cpp
 *
 *  Created on: 2012-3-21
 *      Author: frank
 */

//#include "mt_mem_pool.h"

namespace atlasdb {
  namespace utility {
    mt_mem_pool::mt_mem_pool() {
      typedef __gnu_cxx:: __pool_base ::_Tune tune_type;

      tune_type t_opt(8, 5120, 8, 5120, 20, 10, false);
      _allocator._M_set_options(t_opt);
    }

    mt_mem_pool::~mt_mem_pool() {
      free();
    }

//    void *mt_mem_pool::allocate(size_t length)
//    {
//      return _allocator.allocate(length);
//    }
//
//    void mt_mem_pool::deallocate(void *p, size_t length)
//    {
//      _allocator.deallocate((char*)p, length);
//    }

    char* atlasdb::utility::mt_mem_pool::alloc(size_t length) {
      char* p = _allocator.allocate(length);
      _vUsedMem[p] = length;
      return p;
    }

    void atlasdb::utility::mt_mem_pool::free() {
      for (auto itr = _vUsedMem.begin(); itr != _vUsedMem.end(); ++itr) {
        _allocator.deallocate(itr->first, itr->second);
      }
      _vUsedMem.clear();
    }
  }
}

