#ifndef STORAGE_BASIC_ENVIRONMENT_H_
#define STORAGE_BASIC_ENVIRONMENT_H_

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
    class basic_environment {
    public:

      basic_environment() {}

      basic_environment(const basic_environment& other) = delete;

      basic_environment(const basic_environment& other);

    public:

      bool open(const char* dbhome);
      bool close();
      bool dbremove(const char* name);
      bool dbrename(const char* name, const char* rename);
      bool remove(const char* dbhome);

      //set config before open
      bool adddatadir(const char* dir);
      bool getdatadirs(const char*** dir);
      bool setlgdir(const char* dir);
      bool getlgdir(const char*** dir);

      bool setcachesize(uint gbytes, uint bytes, int ncache);
      bool getcachesize(uint* gbytes, uint* bytes, int* ncache);

      bool setflags(uint flag);
      bool getflags(uint* flag);
      bool setmutexmax(uint max);
      bool getmutexmax(uint* max);
      bool setlkmaxlockers(uint max);
      bool getlkmaxlockers(uint* max);
      bool setlkmaxlocks(uint max);
      bool getlkmaxlocks(uint* max);
      bool setlkmaxobjects(uint max);
      bool getlkmaxobjects(uint* max);

      bool setlgbufsize(uint bytes);
      bool getlgbufsize(uint* bytes);
      bool setlgfilesize(uint max);
      bool getlgfilesize(uint* max);
      bool settxmax(uint max);
      bool gettxmax(uint* max);
      bool setthreadcount(uint max);
      bool getthreadcount(uint* max);
    };

  } // storage
} // atlasdb

#endif // STORAGE_BASIC_LOCAL_ENVIRONMENT_H_
