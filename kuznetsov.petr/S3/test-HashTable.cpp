#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <functional>
#include <utility>
#include "HashTable.hpp"

struct LongComp {
  bool operator()(const long a, const long b) const
  {
    return a == b;
  }
};

BOOST_AUTO_TEST_CASE(Default_Construct_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  BOOST_TEST(map.getSize() == 0);  
  BOOST_TEST(map.getCapacity() == 16);
}
/*
BOOST_AUTO_TEST_CASE(Copy_Construct_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  long keys[] {1, 2, 3, 4};
  for (size_t i = 0; i < 4; ++i) {
    map.add(keys[i], i*2);
  }
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map2(map);
  BOOST_TEST(map.getSize() == map2.getSize());
  BOOST_TEST(map.getCapacity() == map2.getCapacity());
  for (size_t i = 0; i < 4; ++i) {
    BOOST_TEST(map[keys[i]] == map2[keys[i]]);
  }
}
*/
BOOST_AUTO_TEST_CASE(Move_Construct_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map(32);
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map2(std::move(map));
  BOOST_TEST(map.getCapacity() == 0);
  BOOST_TEST(map2.getCapacity() == 32);
}

BOOST_AUTO_TEST_CASE(Copy_Assignment_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map(16);
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map2;

  map2 = map;

  BOOST_TEST(map2.getCapacity() == map.getCapacity());
  BOOST_TEST(map2.getSize() == map.getSize());
}

BOOST_AUTO_TEST_CASE(Move_Assignment_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map(32);
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map2;
  size_t s = map.getSize();
  map2 = std::move(map);
  
  BOOST_TEST(map2.getCapacity() == 32);
  BOOST_TEST(map2.getSize() == s);
  BOOST_TEST(map.getCapacity() == 0);
  BOOST_TEST(map.getSize() == 0);
}

BOOST_AUTO_TEST_CASE(Add_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  BOOST_TEST(map.getSize() == 0);
  map.add(1, 3);   
  BOOST_TEST(map.getSize() == 1);
  map.add(2, 4);   
  BOOST_TEST(map.getSize() == 2);
  map.add(1, 8);   
  BOOST_TEST(map.getSize() == 3);
  
}

BOOST_AUTO_TEST_CASE(Has_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  map.add(1, 21);
  map.add(2, 34);
  BOOST_TEST(map.has(1) == true);
  BOOST_TEST(map.has(3) == false);
  BOOST_TEST(map.has(2) == true);
}

BOOST_AUTO_TEST_CASE(Drop_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  map.add(1, 21);
  map.add(2, 34);
  BOOST_TEST(map.getSize() == 2);
  BOOST_TEST(map.drop(1) == 21);
  BOOST_TEST(map.getSize() == 1);
  BOOST_CHECK_THROW(map.drop(3), std::logic_error);
  BOOST_TEST(map.drop(2) == 34);
  BOOST_TEST(map.getSize() == 0);
  BOOST_CHECK_THROW(map.drop(3), std::logic_error);
}






