#define BOOST_TEST_MODULE module_test_move

#include <iostream>
#include <cassert>
#include <utility>
#include <algorithm>
#include <functional>

#include <boost/test/unit_test.hpp>
#include <atlasdb/utility/data_block.h>

BOOST_AUTO_TEST_SUITE(move_test)

BOOST_AUTO_TEST_CASE( d1 )
{
  using namespace atlasdb;

  char buf[1024];
  std::fill_n(buf, sizeof(buf), '\0');
  sprintf(buf, "%s", "Never, never, never give up");

  // constructor
  raw_block rb(buf, sizeof(buf));

  // move constructor
  raw_block rb2 = std::move(rb);
  raw_block rb3(std::move(rb2));

  // move assignment operator
  raw_block rb4;
  rb4 = std::move(rb3);

  // failed test: assignment constructor
  raw_block rb5, rb6, rb7, rb8;
  // rb5 = rb6 = rb7 = rb8 = rb; // error

  rd_wrapper<raw_block> wrapper(rb4);

  { // do not release the underlying data
    rd_wrapper<raw_block> wrapper2(rb);
  }

  wrapper.address();
  wrapper.size();
  wrapper.data();

  assert(rb4.size());
  assert(wrapper.size());
  assert((char*)wrapper.address() == buf);

  std::cout << (char*)wrapper.address() << std::endl;

  {
    raw_block rb9;
    rb9 = std::move(rb4);
  }

  assert(!rb4.size());

  rd_wrapper<char [1024]> wrapper2(buf);
  rd_wrapper<char [1024]>&& wrapper3 = std::move(wrapper2);
  assert(&wrapper2 == &wrapper3);
}

BOOST_AUTO_TEST_SUITE_END()
