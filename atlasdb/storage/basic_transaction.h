#ifndef STORAGE_BASIC_TRANSACTION_H_
#define STORAGE_BASIC_TRANSACTION_H_

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
     * basic_local_environment<EP> environment;
     * envid = environment.id();
     * basic_storehouse<P> storage(envid)
     *  , storage2(envid)
     *  , storage3(envid);
     * basic_transaction<E>        transaction(envid);
     * basic_transaction<E>        transaction2(envid);
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
    class basic_transaction {
    public:

      basic_transaction() {}

      basic_transaction(const basic_transaction& other) = delete;

      basic_transaction(const basic_transaction& other);

    public:

      bool open(const char* dbhome);
      bool close();
      bool dbremove(const char* name);
      bool dbrename(const char* name, const char* rename);
      bool remove(const char* dbhome);

    };

  } // storage
} // atlasdb

#endif // STORAGE_BASIC_TRANSACTION_H_
