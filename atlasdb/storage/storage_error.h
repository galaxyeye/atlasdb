#ifndef STORAGE_ERROR_CODE_H_
#define STORAGE_ERROR_CODE_H_

#include <string>
#include <system_error>

namespace atlasdb {
  namespace storage {

    enum class errc {
      success,
      buffer_too_small,
      do_not_index,
      foreign_key_conflict,
      heap_full,
      key_empty,
      key_exists,
      lock_deadlock,
      lock_not_granted,
      log_buffer_full,
      log_verifiaction_fail,
      no_server,
      item_not_found,
      db_old_version,
      page_not_found,
      rep_dup_master,
      rep_handle_dead,
      rep_hold_election,
      rep_ignore,
      rep_is_perm,
      rep_join_failure,
      rep_lease_expired,
      rep_lockout,
      rep_new_site,
      rep_not_perm,
      rep_unavail,
      rep_would_rollback,
      run_recovery,
      secondary_bad,
      timeout,
      verify_bad,
      version_mismatch,

      invalid_argument,
      permission_denied,
      no_space_on_device,
      no_file_or_dir,
      device_or_resource_busy,
      bad_address,

      provider_not_exists,
      iterator_not_dereferencable,
      iterator_not_incrementable,
      iterator_not_decrementable,
      indexnum_not_match,
      unknown
    };

    // should be a singleton
    class storage_error_category: public std::error_category {
    public:

      virtual const char *name() const {
        return "storage error category";
      }

      virtual std::string message(int code) const {
        switch (code) {
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

      static const error_category& instance() {
        static storage_error_category instance;
        return instance;
      }

    private:

      storage_error_category() {
      }
      ;
    };
    // errc

    std::error_code make_error_code(errc e) {
      return std::error_code(static_cast<int>(e), storage_error_category::instance());
    }

    class storage_error: public std::runtime_error {
    public:

      storage_error(std::error_code __ec = std::error_code()) :
          std::runtime_error(__ec.message()), _M_code(__ec) {
      }

      storage_error(std::error_code __ec, const std::string& __what) :
          std::runtime_error(__what), _M_code(__ec) {
      }

      /*
       * TODO: Add const char* ctors to all exceptions.
       *
       * system_error(error_code __ec, const char* __what)
       * : runtime_error(__what), _M_code(__ec) { }
       *
       * system_error(int __v, const error_category& __ecat, const char* __what)
       * : runtime_error(__what), _M_code(error_code(__v, __ecat)) { }
       */

      storage_error(int __v, const std::error_category& __ecat) :
          std::runtime_error(""), _M_code(std::error_code(__v, __ecat)) {
      }

      storage_error(int __v, const std::error_category& __ecat, const std::string& __what) :
          std::runtime_error(__what), _M_code(std::error_code(__v, __ecat)) {
      }

      virtual ~storage_error() throw () {
      }

      const std::error_code& code() const throw () {
        return _M_code;
      }

    private:
      std::error_code _M_code;
    };

  } // storage
} // atlasdb

namespace std {
  template<>
  struct is_error_code_enum<atlasdb::storage::errc> : public true_type {
  };
}

#endif // STORAGE_ERROR_CODE_H_
