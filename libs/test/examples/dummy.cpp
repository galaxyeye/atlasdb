#define BOOST_TEST_MODULE dummy_test_module

#include <utility>
#include <string>
#include <set>
#include <map>

#include <boost/test/unit_test.hpp>

namespace A {
  class raw_block {
public:
    char ch;
  };
};

BOOST_AUTO_TEST_SUITE(dummy_test_suite)

BOOST_AUTO_TEST_CASE( d1 )
{
  using A::raw_block;

  typedef std::multimap<raw_block, raw_block> simple_repository;
  typedef std::map<raw_block, raw_block> simple_index;
  typedef std::map<std::string, simple_index> simple_indexer;
  std::set<A::raw_block> s;
  BOOST_CHECK_EQUAL( "str2", "str2" );
}

BOOST_AUTO_TEST_CASE( d2 )
{
  BOOST_CHECK_EQUAL( "boadsfoeoadf", "dueueeee" );
}

BOOST_AUTO_TEST_CASE( d3 )
{
  BOOST_CHECK_EQUAL( std::string(), std::string() );
}

BOOST_AUTO_TEST_SUITE_END()
