#define BOOST_TEST_MODULE dummy1

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(dummy1)

BOOST_AUTO_TEST_CASE( d1 )
{
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
