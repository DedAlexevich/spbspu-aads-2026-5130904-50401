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

BOOST_AUTO_TEST_CASE(CopyAssignmentOperatorTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tr1;
  tr1.push(1, 2);
  kuznetsov::BSTree< int, int, std::less< int > > tr2;
  tr2 = tr1;
  BOOST_TEST(tr2.getSize() == 1);
  BOOST_TEST(tr2.at(1) == 2);
  BOOST_TEST(tr1.getSize() == 1);
}

BOOST_AUTO_TEST_CASE(MoveAssignmentOperatorTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tr1;
  tr1.push(1, 2);
  kuznetsov::BSTree< int, int, std::less< int > > tr2;
  tr2 = std::move(tr1);
  BOOST_TEST(tr2.getSize() == 1);
  BOOST_TEST(tr2.at(1) == 2);
  BOOST_TEST(tr1.getSize() == 0);
}

BOOST_AUTO_TEST_CASE(PushAndAtTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tr1;
  BOOST_TEST(tr1.getSize() == 0);
  tr1.push(143, 2);
  tr1.push(32, 1);
  tr1.push(72, 3);
  BOOST_TEST(tr1.getSize() == 3);
  BOOST_TEST(tr1.at(143) == 2);
  BOOST_TEST(tr1.at(32) == 1);
  BOOST_TEST(tr1.at(72) == 3);
}

BOOST_AUTO_TEST_CASE(DropTest)
{
  kuznetsov::BSTree< int, int, std::less< int > > tr1;
  int ks[] {10, 5, 15, 2, 9, 13};
  int vs[] {1, 2, 3, 4, 5, 6};
  for (size_t i = 0; i < 6; ++i) {
    tr1.push(ks[i], vs[i]);
  }
  BOOST_TEST(tr1.getSize() == 6);
  for (size_t i = 0; i < 6; ++i) {
    tr1.drop(ks[i]);
    BOOST_TEST(tr1.getSize() == 6 - i - 1);
    for (size_t j = 0; j < 6; ++j) {
      if (j <= i) {
        BOOST_CHECK_THROW(tr1.at(ks[j]), std::logic_error);
      } else {
        BOOST_TEST(tr1.at(ks[j]) == vs[j]);
      }
    }
  }
  BOOST_CHECK_THROW(tr1.drop(10), std::logic_error);
}


