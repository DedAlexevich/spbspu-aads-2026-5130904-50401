#define BOOST_TEST_MODULE S4
#include <boost/test/included/unit_test.hpp>
#include "./BSTree.hpp"

BOOST_AUTO_TEST_CASE(DefaultConstructorTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tree;
  BOOST_TEST(tree.getSize() == 0);
  BOOST_TEST(tree.height() == 0);
}

BOOST_AUTO_TEST_CASE(CopyConstructorTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tr1;
  tr1.push(1, 2);
  kuznetsov::BSTree< int, int, std::less< int > > tr2(tr1);
  BOOST_TEST(tr2.getSize() == 1);
  BOOST_TEST(tr2.at(1) == 2);
}

BOOST_AUTO_TEST_CASE(MoveConstructorTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tr1;
  tr1.push(1, 2);
  kuznetsov::BSTree< int, int, std::less< int > > tr2(std::move(tr1));
  BOOST_TEST(tr2.getSize() == 1);
  BOOST_TEST(tr2.at(1) == 2);
  BOOST_TEST(tr1.getSize() == 0);
}





