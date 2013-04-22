#define BOOST_TEST_MODULE ir
#include <string>
#include <list>
#include <utility>
#include <iostream>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/serialization/array.hpp>
#include <boost/iterator_adaptors.hpp>

using namespace std;
using namespace boost;
using namespace boost::property_tree;
using namespace boost::property_tree::json_parser;
using namespace boost::posix_time;

// To validate a json document, go to http://jsonlint.com/

/************************************************* 
Print a tree in readable format like this: 

root
-<level1-key, level1-value>
-<level1-key, level1-value>
--<level2-key, level2-value>
--<level2-key, level2-value>
---<level3-key, level3-value>
-<level1-key, level1-value>
-<level1-key, level1-value>

****************************************************/
void print_ptree(const ptree& pt, int level = 0) {
  if (level == 0) {
    cout << "<" << pt.data() << ">" << endl;
    print_ptree(pt, level + 1);
  }
  else {
    for (ptree::const_iterator it = pt.begin(); it != pt.end(); ++it) {
      int l = level;
      while (l--) cout << "-";
      cout << "<" << it->first << ", " << it->second.data() << ">" << endl;

      print_ptree(it->second, level + 1);
    }
  }
}

BOOST_AUTO_TEST_SUITE(ir)

BOOST_AUTO_TEST_CASE(print)
{
  ifstream ifs("../data.json");
  BOOST_CHECK(ifs.good());
  ptree pt;
  read_json(ifs, pt);

  print_ptree(pt);

  cout << "+---------------------------+" << endl;
}

// TODO:
BOOST_AUTO_TEST_CASE(parse_performance)
{
  ifstream ifs("../data.json");
  BOOST_CHECK(ifs.good());
  stringstream ss;
  ss << ifs.rdbuf();
  ifs.close();

  ptime t = microsec_clock::universal_time();
  int loops = 10000 * 10000;
  while (loops--) {
    // do some test here
  }
  ptime t2 = microsec_clock::universal_time();

  ptime::time_duration_type duration = t2 - t;
  cout << "Performance test result: " << duration << "ms";
}

BOOST_AUTO_TEST_SUITE_END()
