#include <atlasdb/storage/bdb_provider.h>
#include <atlasdb/storage/basic_storehouse.h>
#include <atlasdb/storage/basic_indexer.h>
#include <atlasdb/storage/basic_index.h>

#include <atlasdb/utility/ptree_printer.h>
#include <atlasdb/storage/utility/bdb_io.h>
#include <atlasdb/query/manipulation.h>
#include <atlasdb/rpc/protocol/json/translator.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>

using namespace std;
using namespace boost;
using namespace boost::property_tree;
using namespace boost::property_tree::json_parser;

using namespace atlasdb;
using namespace atlasdb::storage;
using namespace atlasdb::storage::provider;

static const std::string dbname = "basic";
static const std::string dbname1 = "basic_index1";
static const std::string dbname2 = "basic_index2";

class archiver {

  typedef atlasdb::storage::transaction        transaction;
  typedef atlasdb::storage::environment        environment;
  typedef atlasdb::storage::storehouse          storehouse;
  typedef atlasdb::storage::repository      repository;
  typedef atlasdb::storage::index           index;
  typedef atlasdb::storage::indexer         indexer;

  typedef typename repository::key_type key_t;
  typedef typename repository::data_type data_t;
  typedef typename repository::index_key_type index_key_t;

  typedef query::functor::function_data<storehouse> function_data;
  typedef query::functor::table_manipulator<storehouse> table_manipulator;
  typedef query::functor::entity_manipulator<storehouse> entity_manipulator;
  typedef rpc::json::translator<storehouse> translator;

public:

  archiver() : _is_env_open(false) {}
  ~archiver() { _env.close(); }

public:

  void prepare() {
    if (!_is_env_open) {
      _env.open("./env/");
      _is_env_open = true;
    }

    try {
      do_prepare();
    }
    catch(std::bad_alloc& e) {
      // TODO
      std::cout << e.what() << std::endl;
    }
    catch(std::exception& e) {
      // TODO
      std::cout << e.what() << std::endl;
    }
    catch(std::string& e) {
      std::cout << e << std::endl;
    }
    catch(const char* ch) {
      std::cout << ch << std::endl;
    }
    catch(...) {
      std::cout << "Unknown error" << std::endl;
    }
  }

  void load() {
    if (!_is_env_open) {
      _env.open("./env/");
      _is_env_open = true;
    }

    try {
      do_load();
    }
    catch(std::bad_alloc& e) {
      // TODO
      std::cout << e.what() << std::endl;
    }
    catch(std::exception& e) {
      // TODO
      std::cout << e.what() << std::endl;
    }
    catch(std::string& e) {
      std::cout << e << std::endl;
    }
    catch(const char* ch) {
      std::cout << ch << std::endl;
    }
    catch(...) {
      std::cout << "Unknown error" << std::endl;
    }
  }

public:

  bdb_environment& env() { return _env; }

  char *result_buf;
  size_t buf_size;

protected:

  void do_insert() {
    ifstream ifs("insert.json");
    ptree pt;
    read_json(ifs, pt);

    translator translator;
    function_data data = translator.process(pt);

    // for(int i=0; i<10; i++) {
      entity_manipulator::inserter inserter(data);
      inserter(_env);
    // }
  }

  void do_prepare() {
    unsigned long long start = 0;
    unsigned long long end = 0;
    struct timeval tv;

    std::cout << "put: begin------" << std::endl;
    gettimeofday(&tv, nullptr);
    start = tv.tv_sec * 1000 * 1000 + tv.tv_usec;

    do_insert();

    gettimeofday(&tv, nullptr);
    end = tv.tv_sec*1000*1000+tv.tv_usec;
    std::cout << " time=" << (end-start) / 1000 << std::endl;
    std::cout << "put: end------" << std::endl;
  }

  void do_prepare_2() {
    unsigned long long start = 0;
    unsigned long long end = 0;
    struct timeval tv;
    int count = 0;
    char buf[1024];
    snprintf(buf, 1024, "%s", "abcdefghijklmnopqrstuvwxyz");
    // int len = 1024;
    int id =0;
    int v = 0;
    std::vector<index_key_t> indexkeys;
    index_key_t indexkey1(&v, sizeof(int));
    // indexkeys.push_back(std::move(indexkey1));

    storehouse storage;
    storage.repository::open(_env, dbname);
    bdb_transaction txn;
    _env.txnbegin(txn);
    std::cout << "put: begin------" << std::endl;
    gettimeofday(&tv, nullptr);
    start = tv.tv_sec * 1000 * 1000 + tv.tv_usec;
    for(int i=0; i<100; i++) {
      id = i * 2 + 1;
      key_t key(&id, sizeof(int));
      data_t data(buf, sizeof(buf));

      v = id = i;
      storage.put(txn, key, data, indexkeys);
      count++;
    }

    txn.commit();

    gettimeofday(&tv, nullptr);
    end = tv.tv_sec*1000*1000+tv.tv_usec;
    std::cout << " time=" << (end-start) / 1000 << std::endl;
    std::cout << "put: end------" << std::endl;
  }

  void do_load() {
    ifstream ifs("load.json");
    ptree pt;
    read_json(ifs, pt);

    translator translator;
    function_data data = translator.process(pt);

    buf_size = 1024u * 1024u;
    result_buf = new char[buf_size];
    std::fill_n(result_buf, buf_size, '\0');

    table_manipulator::loader loader(data, result_buf, buf_size);
    loader(_env);
  }

  void do_test_2() {
    std::cout << "*************************test_transaction begin*************************" << std::endl;
    int len = 1024 * 10000;
    buf_size = len;
    result_buf = new char[buf_size];
    std::fill_n(result_buf, len, '\0');

    //int ret = 0;
    int count = 0;
    unsigned long long start = 0;
    unsigned long long end = 0;
    struct timeval tv;

    storehouse storage;
    storage.repository::open(_env, dbname);

    bdb_transaction txn;

    std::cout << "repository transaction begin+++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    storage.set_iter_txn(txn);
    auto repositoryNoRef = storage.repository::begin();
    auto repositoryNoRef_end = storage.repository::end();
    repositoryNoRef.setbuf(result_buf, len);
    std::cout << "repository iterator: begin forword(++) no reference------" << std::endl;

    count = 0;

    gettimeofday(&tv, nullptr);
    start = tv.tv_sec*1000*1000+tv.tv_usec;
    for(; repositoryNoRef != repositoryNoRef_end; ++repositoryNoRef) {
      count++;
    }

    gettimeofday(&tv, nullptr);
    end = tv.tv_sec*1000*1000+tv.tv_usec;
    std::cout << "count=" << count << " time=" << (end - start) / 1000 << std::endl;
    std::cout << "repository iterator: end------"<< std::endl;
//    --repositoryNoRef;
//    auto value = *repositoryNoRef;
//    std::cout << "value size=" << value.second.size() << std::endl;

    txn.commit();
    storage.close();

    std::cout << result_buf << std::endl;
  }

private:
  bool _is_env_open;
  bdb_environment _env;
};
