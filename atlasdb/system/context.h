/*
 * context.h
 *
 *  Created on: Apr 30, 2013
 *      Author: vincent
 */

#ifndef ATLAS_SYSTEM_CONTEXT_H_
#define ATLAS_SYSTEM_CONTEXT_H_

#include <atomic>
#include <mutex>

namespace atlas {
  namespace system {

    struct context {
      static std::atomic<unsigned long long> mutex_limit;
      static std::atomic<unsigned long long> memory_limit;

      static std::mutex mutex;
    };

    std::mutex context::mutex;
  } // system
} // atlas

#endif /* ATLAS_SYSTEM_CONTEXT_H_ */
