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
    LIter& operator++()
    {
      this->curr = this->curr->next;
      return this;
    }
    LIter& operator++(int)
    {
      LIter temp(*this);
      ++(*this);
      return temp;
    }
    LIter& operator--()
    {
      this->curr = this->curr->prev;
      return this;
    }
    LIter& operator--(int)
    {
      LIter temp(*this);
      --(*this);
      return temp;
    }
    T& operator*()
    {
      return curr->val;
    }
    bool operator==(const LIter& y)
    {
      return this->curr == y.curr;
    }

    bool operator!=(const LIter& y)
    {
      return !(this == y);
    }
  };


}

#endif