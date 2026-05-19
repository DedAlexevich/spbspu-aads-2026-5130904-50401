#define BOOST_TEST_MODULE S4
#include <boost/test/included/unit_test.hpp>
#include "./BSTree.hpp"

BOOST_AUTO_TEST_CASE(DefaultConstructorTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tree;
  BOOST_TEST(tree.getSize() == 0);
  BOOST_TEST(tree.height() == 0);
}



