#define BOOST_TEST_MODULE dummy

#include <boost/test/unit_test.hpp>
#include <atlasdb/serialization/cson/util/csonassert.h>

using namespace cson;

BOOST_AUTO_TEST_SUITE(dummy)

BOOST_AUTO_TEST_CASE( dummy )
{
  cson_assert(1 == 2, "1 == 2");
  BOOST_CHECK_EQUAL( "str2", "str2" );
}

BOOST_AUTO_TEST_SUITE_END()
