#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
#include <iostream>

namespace kuznetsov {
  namespace detail {
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
    LCIter(detail::Node< T >* pn);

    LCIter& operator++();
    LCIter operator++(int);

    LCIter& operator--();
    LCIter operator--(int);

    const T& operator*() const;
    const T* operator->() const;

    bool operator==(const LCIter& y) const noexcept;
    bool operator!=(const LCIter& y) const noexcept;
  private:
    friend class List< T >;
    detail::Node< T >* curr_;
  };

  template< class T >
  class LIter {
  public:
    LIter(detail::Node< T >* pn);

    LIter& operator++();
    LIter operator++(int);

    LIter& operator--();
    LIter operator--(int);

    T& operator*();
    T* operator->();

    bool operator==(const LIter& y) const noexcept;
    bool operator!=(const LIter& y) const noexcept;
  private:
    friend class List< T >;
    detail::Node< T >* curr_;
  };

  template< class T >
  class LRCIter {
  public:
    LRCIter(detail::Node< T >* pn);

    LRCIter& operator++()
    {
      if (curr_) {
        curr_ = curr_->prev_;
      }
      return *this;
    }

    LRCIter operator++(int)
    {
      LRCIter temp(*this);
      ++(*this);
      return temp;
    }

    LRCIter& operator--()
    {
      if (curr_) {
        curr_ = curr_->next_;
      }
      return *this;
    }

    LRCIter operator--(int)
    {
      LRCIter temp(*this);
      --(*this);
      return temp;
    }

    const T& operator*() const
    {
      if (!this->curr_) {
        throw std::logic_error("Null iterator");
      }
      return curr_->val_;
    }

    bool operator==(const LRCIter& y) const
    {
      return curr_ == y.curr_;
    }

    bool operator!=(const LRCIter& y) const
    {
      return !(*this == y);
    }

  protected:
    friend class List< T >;
    detail::Node< T >* curr_;
  };

  template< class T >
  class LRIter: public LRCIter< T > {
  public:
    LRIter(detail::Node< T >* pn):
      LRCIter< T >(pn)
    {}

    T& operator*() {
      if (!this->curr_) {
        throw std::logic_error("Null iterator");
      }
      return this->curr_->val_;
    }
  };

  template< class T >
  class List {
  public:
    List(): head_(nullptr), size_(0)
    {}

    List(const List& other):
      head_(nullptr),
      size_(0)
    {
      if (other.empty()) {
        return;
      }
      detail::Node<T>* current = other.head_;
      do {
        try {
          insert(cend(), current->val_);
          current = current->next_;
        } catch (...) {
          std::cerr << "Copy construct error\n";
          clear();
          throw;
        }
      } while (current != other.head_);
    }

    List(List&& other) noexcept :
      head_(other.head_),
      size_(other.size_)
    {
      other.head_ = nullptr;
      other.size_ = 0;
    }

    ~List()
    {
      clear();
    }

    List& operator=(const List& other)
    {
      if (this == &other) {
        return *this;
      }
      clear();
      if (other.empty()) {
        return *this;
      }
      detail::Node< T >* current = other.head_;
      do {
        try {
          insert(cend(), current->val_);
          current = current->next_;
        } catch (...) {
          std::cerr << "Copy = error\n";
          clear();
          throw;
        }
      } while (current != other.head_);

      return *this;
    }

    List& operator=(List&& other) noexcept
    {
      if (this == &other) {
        return *this;
      }

      clear();

      head_ = other.head_;
      size_ = other.size_;

      other.head_ = nullptr;
      other.size_ = 0;
      return *this;
    }

    LIter< T > insert(LCIter< T > it, const T& val)
    {
      detail::Node< T >* n = new detail::Node< T >{val, nullptr, nullptr};

      if (head_ == nullptr) {
        head_ = n;
        n->next_ = n;
        n->prev_ = n;
      } else if (!it.curr_) {
        detail::Node< T >* tail = head_->prev_;

        n->next_ = head_;
        n->prev_ = tail;
        tail->next_ = n;
        head_->prev_ = n;
      } else {
        detail::Node< T >* current = it.curr_;
        detail::Node< T >* prev = current->prev_;

        n->next_ = current;
        n->prev_ = prev;
        prev->next_ = n;
        current->prev_ = n;

        if (current == head_) {
          head_ = n;
        }
      }

      size_++;
      return LIter< T >(n);
    }

    LIter< T > insert(LCIter< T > it, T&& val)
    {
      detail::Node< T >* n = new detail::Node< T >{std::move(val), nullptr, nullptr};

      if (head_ == nullptr) {
        head_ = n;
        n->next_ = n;
        n->prev_ = n;
      } else if (!it.curr_) {
        detail::Node< T >* tail = head_->prev_;

        n->next_ = head_;
        n->prev_ = tail;
        tail->next_ = n;
        head_->prev_ = n;
      } else {
        detail::Node< T >* current = it.curr_;
        detail::Node< T >* prev = current->prev_;

        n->next_ = current;
        n->prev_ = prev;
        prev->next_ = n;
        current->prev_ = n;

        if (current == head_) {
          head_ = n;
        }
      }

      size_++;
      return LIter< T >(n);
    }

    T& front()
    {
      if (!head_) {
        throw std::logic_error("Empty list");
      }
      return head_->val_;
    }

    T& back()
    {
      if (!head_) {
        throw std::logic_error("Empty list");
      }
      return head_->prev_->val_;
    }

    const T& front() const
    {
      if (!head_) {
        throw std::logic_error("Empty list");
      }
      return head_->val_;
    }

    const T& back() const
    {
      if (!head_) {
        throw std::logic_error("Empty list");
      }
      return head_->prev_->val_;
    }

    void popFront()
    {
      if (!head_) {
        throw std::logic_error("Empty list");
      }
      erase(cbegin());
    }

    void popBack()
    {
      if (!head_) {
        throw std::logic_error("Empty list");
      }
      erase(LCIter< T >(head_->prev_));
    }

    LIter< T > begin()
    {
      return LIter< T >(head_);
    }

    LIter< T > end()
    {
      return LIter< T >(nullptr);
    }

    LCIter< T > cbegin() const
    {
      return LCIter< T >(head_);
    }

    LCIter< T > cend() const
    {
      return LCIter< T >(nullptr);
    }

    LRIter< T > rbegin()
    {
      return LRIter< T >(head_->prev_);
    }

    LRIter< T > rend()
    {
      return LRIter< T >(nullptr);
    }

    LRCIter< T > rcbegin() const
    {
      return LRCIter< T >(head_->prev_);
    }

    LRCIter< T > rcend() const
    {
      return LRCIter< T >(nullptr);
    }

    void clear()
    {
      if (!head_) {
        return;
      }
      detail::Node< T >* curr = head_;
      do {
        detail::Node< T >* next = curr->next_;
        delete curr;
        curr = next;
      } while (curr != head_);
      size_ = 0;
      head_ = nullptr;
    }

    LIter< T > erase(LCIter< T > it)
    {
      if (!head_ || !it.curr_) {
        throw std::logic_error("Empty list or iterator");
      }
      detail::Node< T >* nextNode = it.curr_->next_;
      bool f = (it.curr_ == head_);

      if (size_ == 1) {
        delete it.curr_;
        head_ = nullptr;
        size_ = 0;
        return LIter< T >(nullptr);
      }

      it.curr_->prev_->next_ = it.curr_->next_;
      it.curr_->next_->prev_ = it.curr_->prev_;
      delete it.curr_;
      --size_;

      if (f) {
        head_ = nextNode;
      }

      return LIter< T >(nextNode);
    }

    size_t size() const
    {
      return size_;
    }

    bool empty() const
    {
      return !size_;
    }

  private:
    detail::Node< T >* head_;
    size_t size_;
  };

}

template< class T >
kuznetsov::LCIter< T >::LCIter(kuznetsov::detail::Node< T >* pn):
  curr_(pn)
{}

template< class T >
kuznetsov::LCIter< T >& kuznetsov::LCIter< T >::operator++()
{
  if (curr_) {
    curr_ = curr_->next_;
  }
  return *this;
}

template< class T >
kuznetsov::LCIter< T > kuznetsov::LCIter< T >::operator++(int)
{
  LCIter temp(*this);
  ++(*this);
  return temp;
}

template< class T >
kuznetsov::LCIter< T >& kuznetsov::LCIter< T >::operator--()
{
  if (curr_) {
    curr_ = curr_->prev_;
  }
  return *this;
}

template< class T >
kuznetsov::LCIter< T > kuznetsov::LCIter< T >::operator--(int)
{
  LCIter temp(*this);
  --(*this);
  return temp;
}

template< class T >
const T& kuznetsov::LCIter< T >::operator*() const
{
  if (!this->curr_) {
    throw std::logic_error("Null iterator");
  }
  return curr_->val_;
}

template< class T >
const T* kuznetsov::LCIter< T >::operator->() const
{
  if (!this->curr_) {
    throw std::logic_error("Null iterator");
  }
  return &curr_->val_;
}

template< class T >
bool kuznetsov::LCIter< T >::operator==(const LCIter& y) const noexcept
{
  return this->curr_ == y.curr_;
}

template< class T >
bool kuznetsov::LCIter< T >::operator!=(const LCIter& y) const noexcept
{
  return !(*this == y);
}

template< class T >
kuznetsov::LRCIter< T >::LRCIter(detail::Node< T >* pn):
  curr_(pn)
{}
//------------------------------
template< class T >
kuznetsov::LIter< T >::LIter(kuznetsov::detail::Node< T >* pn):
  curr_(pn)
{}

template< class T >
kuznetsov::LIter< T >& kuznetsov::LIter< T >::operator++()
{
  if (curr_) {
    curr_ = curr_->next_;
  }
  return *this;
}

template< class T >
kuznetsov::LIter< T > kuznetsov::LIter< T >::operator++(int)
{
  LIter temp(*this);
  ++(*this);
  return temp;
}

template< class T >
kuznetsov::LIter< T >& kuznetsov::LIter< T >::operator--()
{
  if (curr_) {
    curr_ = curr_->prev_;
  }
  return *this;
}

template< class T >
kuznetsov::LIter< T > kuznetsov::LIter< T >::operator--(int)
{
  LIter temp(*this);
  --(*this);
  return temp;
}

template< class T >
T& kuznetsov::LIter< T >::operator*()
{
  if (!this->curr_) {
    throw std::logic_error("Null iterator");
  }
  return curr_->val_;
}

template< class T >
T* kuznetsov::LIter< T >::operator->()
{
  if (!this->curr_) {
    throw std::logic_error("Null iterator");
  }
  return &curr_->val_;
}

template< class T >
bool kuznetsov::LIter< T >::operator==(const LIter& y) const noexcept
{
  return this->curr_ == y.curr_;
}

template< class T >
bool kuznetsov::LIter< T >::operator!=(const LIter& y) const noexcept
{
  return !(*this == y);
}



#endif

