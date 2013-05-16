/*
 * status.h
 *
 *  Created on: Apr 30, 2013
 *      Author: vincent
 */

#ifndef ATLAS_SYSTEM_STATUS_H_
#define ATLAS_SYSTEM_STATUS_H_

#include <atomic>

namespace atlas {
  namespace system {

    struct status {
      static std::atomic<unsigned long long> requests;
      static std::atomic<unsigned long long> creates;
      static std::atomic<unsigned long long> reads;
      static std::atomic<unsigned long long> updates;
      static std::atomic<unsigned long long> deletes;

      static std::atomic<unsigned long long> storage_errors;
      static std::atomic<unsigned long long> query_errors;
      static std::atomic<unsigned long long> db_errors;
      static std::atomic<unsigned long long> net_errors;
    };

    std::atomic<unsigned long long> status::requests = ATOMIC_VAR_INIT(0);
    std::atomic<unsigned long long> status::creates = ATOMIC_VAR_INIT(0);
    std::atomic<unsigned long long> status::reads = ATOMIC_VAR_INIT(0);
    std::atomic<unsigned long long> status::updates = ATOMIC_VAR_INIT(0);
    std::atomic<unsigned long long> status::deletes = ATOMIC_VAR_INIT(0);

    std::atomic<unsigned long long> status::storage_errors = ATOMIC_VAR_INIT(0);
    std::atomic<unsigned long long> status::query_errors = ATOMIC_VAR_INIT(0);
    std::atomic<unsigned long long> status::db_errors = ATOMIC_VAR_INIT(0);
    std::atomic<unsigned long long> status::net_errors = ATOMIC_VAR_INIT(0);

  } // system
} // atlas

#endif /* ATLAS_SYSTEM_STATUS_H_ */
