/*
 * storage_error.ipp
 *
 *  Created on: May 11, 2013
 *      Author: vincent
 */

#include <atlasdb/storage/storage_error.h>

namespace atlasdb {
  namespace storage {

    class storage_error_category : public std::error_category {
    public:

      virtual const char *name() const noexcept {
        return "storage error category";
      }

      virtual std::string message(int code) const noexcept {
        errc c = static_cast<errc>(code);
        switch (c) {
        case errc::success:
          return "Sotrage operator is successful.";
        case errc::buffer_too_small:
          return "User memory too small for return.";
        case errc::do_not_index:
          return "\"Null\" return from 2ndary callbk";
        case errc::foreign_key_conflict:
          return "A foreign db constraint triggered.";
        case errc::heap_full:
          return "No free space in a heap file.";
        case errc::key_empty:
          return "Key/data deleted or never created.";
        case errc::key_exists:
          return "The key/data pair already exists.";
        case errc::lock_deadlock:
          return "Deadlock";
        case errc::lock_not_granted:
          return "Lock unavailable.";
        case errc::log_buffer_full:
          return "In-memory log buffer full.";
        case errc::log_verifiaction_fail:
          return "Log verification failed.";
        case errc::no_server:
          return "Server panic return.";
        case errc::item_not_found:
          return "Key/data pair not found.";
        case errc::db_old_version:
          return "Out-of-date version.";
        case errc::page_not_found:
          return "Requested page not found.";
        case errc::rep_dup_master:
          return "There are two masters.";
        case errc::rep_handle_dead:
          return "Rolled back a commit.";
        case errc::rep_hold_election:
          return "Time to hold an election.";
        case errc::rep_ignore:
          return "This msg should be ignored.";
        case errc::rep_is_perm:
          return "Cached not written perm written.";
        case errc::rep_join_failure:
          return "Unable to join replication group. ";
        case errc::rep_lease_expired:
          return "Master lease has expired.";
        case errc::rep_lockout:
          return "API/Replication lockout now.";
        case errc::rep_new_site:
          return "New site entered system.";
        case errc::rep_not_perm:
          return "Permanent log record not written.";
        case errc::rep_unavail:
          return "Site cannot currently be reached.";
        case errc::rep_would_rollback:
          return "UNDOC: rollback inhibited by app.";
        case errc::run_recovery:
          return "Panic return.";
        case errc::secondary_bad:
          return "econdary index corrupt.";
        case errc::timeout:
          return "Timed out on read consistency.";
        case errc::unknown:
        default:
          return "Unknown storage error.";
        }
      }

    };

    const std::error_category& get_storage_category() {
      static storage_error_category instance;
      return instance;
    }

  } // storage
} // atlasdb
