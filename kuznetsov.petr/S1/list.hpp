#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
namespace kuznetsov {

  namespace {
    template< class T >
    struct Node {
      T val_;
      Node< T >* next_;
      Node< T >* prev_;
    };
  }

  template< class T >
  class List;

  template< class T >
  class LCIter {
  public:
    LCIter& operator++()
    {
      this->curr_ = this->curr_->next_;
      return this;
    }
    LCIter& operator++(int)
    {
      LCIter temp(*this);
      ++(*this);
      return temp;
    }
    LCIter& operator--()
    {
      this->curr_ = this->curr_->prev_;
      return this;
    }
    LCIter& operator--(int)
    {
      LCIter temp(*this);
      --(*this);
      return temp;
    }
    const T& operator*()
    {
      return curr_->val;
    }
    bool operator==(const LCIter& y)
    {
      return this->curr_ == y.curr_;
    }

    bool operator!=(const LCIter& y)
    {
      return !(this == y);
    }
  private:
    friend class List< T >;
    Node< T >* curr_;
  };

  template< class T >
  class LIter : LCIter< T > {
    T& operator*()
    {
      return curr_->val;
    }
  };

  template< class T >
  class List {
  public:
    List(): head_(nullptr), size_(0) {}
    ~List()
    {
      clear();
    }
    LIter< T > insert(LCIter< T > it, T& val);
    T& front();
    T& back();
    void popFront();
    void popBack();
    void pop(LIter< T > it);
    LIter< T > begin();
    LIter< T > end();
    LCIter< T > cbegin();
    LCIter< T > cend();
    void clear()
    {
      Node< T >* t = nullptr;
      for (size_t i = 0; i < size_; ++i) {
        t = head_->next_;
        delete head_;
        head_ = t;
      }
      size_ = 0;
      head_ = nullptr;
    }
    LIter< T > erase(LCIter< T > it)
    {
      LIter< T > t(it.curr_->next_);
      it.curr_->prev_->next_ = it.curr_->next_;
      it.curr_->next_->prev_ = it.curr_->prev_;
      delete it.curr_;
      return t;
    }

  private:
    Node< T >* head_;
    size_t size_;

  };

}

#endif