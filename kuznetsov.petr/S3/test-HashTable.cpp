#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <functional>
#include "HashTable.hpp"

bool comp(long a, long b)
{
  return a == b;
}

BOOST_AUTO_TEST_CASE(createDefaultHashTable)
{
  kuznetsov::HashTable< long, int, std::hash, comp> map;
  BOOST_TEST(map.getSize() == 0);  
  BOOST_TEST(map.getCapacity() == 10);
}


