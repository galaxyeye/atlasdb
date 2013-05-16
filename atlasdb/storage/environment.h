#ifndef ATLASDB_STORAGE_ENVIRONMENT_H_
#define ATLASDB_STORAGE_ENVIRONMENT_H_

#include <string>
#include <vector>
#include <memory>
#include <cassert>

namespace atlasdb {
  namespace storage {

    using std::string;

    /**
     * A local environment should be explicitly built if multiple repository
     * are manipulated together
     *
     * The word "local" means that this environment is designed to run in
     * a single thread
     *
     * usage :
     *
     * local_environment<EP> environment;
     * envid = environment.id();
     * storehouse<P> storage(envid), storage2(envid), storage3(envid);
     * transaction<E>        transaction(envid);
     * transaction<E>        transaction2(envid);
     *
     * environment.open();
     *
     * transaction.begin();
     *
     * try {
     *  storage.put(...);
     *  storage2.put(...);
     *  storage3.put(...);
     *  storage2.get(...);
     *
     *  transaction.commit();
     * }
     * catch(e) {
     *  transaction.abort();
     * }
     *
     * transaction2.begin();
     * ...
     * transaction2.commit();
     *
     * environment.close();
     *
     * */
    class environment {
    public:

      environment() {}
      environment(const environment& other) = delete;
      environment(const environment& other);

    public:

      void open(const char* home);
      void close();
      void remove(const char* name);
      void rename(const char* name, const char* rename);
      void remove_all(const char* home);

    private:

      size_t _cache_size;
      size_t _max_lock;
      size_t _max_mutex;
      size_t _page_size;
    };

  } // storage
} // atlasdb

#endif // ATLASDB_STORAGE_ENVIRONMENT_H_
