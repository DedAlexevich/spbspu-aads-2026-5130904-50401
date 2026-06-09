#include <HashTable.hpp>
#include <boost/test/unit_test.hpp>
#include <cstddef>
#include <functional>
#include <iostream>
#include <utility>

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

BOOST_AUTO_TEST_CASE(Copy_Construct_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  long keys[]{ 1, 2, 3, 4 };
  for (size_t i = 0; i < 4; ++i) {
    map.add(keys[i], i * 2);
  }
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map2(map);
  BOOST_TEST(map.getSize() == map2.getSize());
  BOOST_TEST(map.getCapacity() == map2.getCapacity());
  for (size_t i = 0; i < 4; ++i) {
    BOOST_TEST(map.at(keys[i]) == map2.at(keys[i]));
  }
}

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
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map(2);
  BOOST_TEST(map.getSize() == 0);
  map.add(1, 3);
  BOOST_TEST(map.getSize() == 1);
  BOOST_CHECK_THROW(map.add(1, 4), std::logic_error);
  map.add(2, 2);
  BOOST_TEST(map.getSize() == 2);
  BOOST_CHECK_THROW(map.add(3, 4), std::logic_error);
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
  map.remove(1);
  BOOST_TEST(map.getSize() == 1);
  BOOST_CHECK_THROW(map.remove(3), std::logic_error);
  map.remove(2);
  BOOST_TEST(map.getSize() == 0);
  BOOST_CHECK_THROW(map.remove(3), std::logic_error);
}

BOOST_AUTO_TEST_CASE(Rehash_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map(4);
  map.add(1, 2);
  map.add(2, 3);
  map.add(3, 4);
  BOOST_TEST(map.getCapacity() == 4);
  map.rehash();
  BOOST_TEST(map.getCapacity() == 8);
  BOOST_TEST(map.getSize() == 3);
}

BOOST_AUTO_TEST_CASE(At_Method_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  map.add(1, 34);
  map.add(2, 52);
  map.add(3, 67);
  BOOST_TEST(map.at(1) == 34);
  BOOST_TEST(map.at(2) == 52);
  BOOST_TEST(map.at(3) == 67);
  BOOST_CHECK_THROW(map.at(4), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(At_Const_Method_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  map.add(1, 34);
  map.add(2, 52);
  map.add(3, 67);
  const kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map2(map);
  BOOST_TEST(map2.at(1) == 34);
  BOOST_TEST(map2.at(2) == 52);
  BOOST_TEST(map2.at(3) == 67);
  BOOST_CHECK_THROW(map2.at(4), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(Iterators_Test)
{
  kuznetsov::HashTable< long, int, std::hash< long >, LongComp > map;
  map.add(1, 34);
  map.add(2, 52);
  map.add(3, 67);
  long keys[]{ 1, 2, 3 };
  int values[]{ 34, 52, 67 };
  size_t i = 0;
  kuznetsov::HTIterator< long, int, true > it = map.cbegin();
  for (; i < 3; ++i) {
    BOOST_TEST(it->first == keys[i]);
    BOOST_TEST((*it).second == values[i]);
    ++it;
  }
}
