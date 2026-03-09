#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
namespace kuznetsov {

  namespace {
    template< class T >
    struct Node {
      T val;
      Node< T >* next;
      Node< T >* prev;
    };
  }

  template< class T >
  class List;

  template< class T >
  class LIter {
    friend class List< T >;
    Node< T >* curr;
  public:
    LIter& operator++();
    LIter& operator++(int);
    LIter& operator--();
    LIter& operator--(int);
    T& operator*();
    bool operator==(const List& __x, const List& __y);
    bool operator!=(const List& __x, const List& __y);
  };


}

#endif