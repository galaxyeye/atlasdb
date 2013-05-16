#ifndef ATLASDB_STORAGE_ERROR_H_
#define ATLASDB_STORAGE_ERROR_H_

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

    extern const std::error_category& get_storage_category();

    static const std::error_category& storage_category = get_storage_category();

    std::error_code make_error_code(errc e) {
      return std::error_code(static_cast<int>(e), get_storage_category());
    }

    class storage_error: public std::runtime_error {
    public:

      storage_error(std::error_code ec = std::error_code()) :
          std::runtime_error(ec.message()), _code(ec) {
      }

      storage_error(std::error_code ec, const std::string& what) :
          std::runtime_error(what), _code(ec) {
      }

      /*
       * TODO: Add const char* ctors to all exceptions.
       *
       * system_error(error_code ec, const char* what)
       * : runtime_error(what), _code(ec) { }
       *
       * system_error(int v, const error_category& ecat, const char* what)
       * : runtime_error(what), _code(error_code(v, ecat)) { }
       */

      storage_error(int v, const std::error_category& ecat)
        : std::runtime_error(""), _code(std::error_code(v, ecat))
      {}

      storage_error(int v, const std::error_category& ecat, const std::string& what)
        : std::runtime_error(what), _code(std::error_code(v, ecat))
      {}

      virtual ~storage_error() noexcept {}

      const std::error_code& code() const noexcept { return _code; }

    private:

      std::error_code _code;
    };

  } // storage
} // atlasdb

namespace std {
  template<>
  struct is_error_code_enum<atlasdb::storage::errc> : public true_type {
  };
}

#endif // ATLASDB_STORAGE_ERROR_H_
