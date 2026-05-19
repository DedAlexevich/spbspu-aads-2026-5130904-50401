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

    LRCIter& operator++();

    LRCIter operator++(int);

    LRCIter& operator--();

    LRCIter operator--(int);

    const T& operator*() const;
    const T* operator->() const;

    bool operator==(const LRCIter& y) const noexcept;

    bool operator!=(const LRCIter& y) const noexcept;

  private:
    friend class List< T >;
    detail::Node< T >* curr_;
  };

  template< class T >
  class LRIter {
  public:
    LRIter(detail::Node< T >* pn);

    LRIter& operator++();

    LRIter operator++(int);

    LRIter& operator--();

    LRIter operator--(int);

    T& operator*();
    T* operator->();

    bool operator==(const LRIter& y) const noexcept;

    bool operator!=(const LRIter& y) const noexcept;

  private:
    friend class List< T >;
    detail::Node< T >* curr_;
  };

  template< class T >
  class List {
  public:
    List();

    List(const List& other);

    List(List&& other) noexcept;

    ~List() noexcept;

    List& operator=(const List& other);

    List& operator=(List&& other) noexcept;

    LIter< T > insert(LCIter< T > it, const T& val);

    LIter< T > insert(LCIter< T > it, T&& val);

    T& front();

    T& back();

    const T& front() const;

    const T& back() const;

    void popFront();

    void popBack();

    LIter< T > begin();

    LIter< T > end();

    LCIter< T > cbegin() const;

    LCIter< T > cend() const;

    LRIter< T > rbegin();

    LRIter< T > rend();

    LRCIter< T > rcbegin() const;

    LRCIter< T > rcend() const;

    void clear();

    LIter< T > erase(LCIter< T > it);

    size_t size() const;

    bool empty() const;

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

template< class T >
kuznetsov::LRCIter< T >::LRCIter(detail::Node< T >* pn):
  curr_(pn)
{}

template< class T >
kuznetsov::LRCIter< T >& kuznetsov::LRCIter< T >::operator++()
{
  if (curr_) {
    curr_ = curr_->prev_;
  }
  return *this;
}

template< class T >
kuznetsov::LRCIter< T > kuznetsov::LRCIter< T >::operator++(int)
{
  LRCIter temp(*this);
  ++(*this);
  return temp;
}

template< class T >
kuznetsov::LRCIter< T >& kuznetsov::LRCIter< T >::operator--()
{
  if (curr_) {
    curr_ = curr_->next_;
  }
  return *this;
}

template< class T >
kuznetsov::LRCIter< T > kuznetsov::LRCIter< T >::operator--(int)
{
  LRCIter temp(*this);
  --(*this);
  return temp;
}

template< class T >
const T& kuznetsov::LRCIter< T >::operator*() const
{
  if (!this->curr_) {
    throw std::logic_error("Null iterator");
  }
  return curr_->val_;
}

template< class T >
const T* kuznetsov::LRCIter< T >::operator->() const
{
  if (!this->curr_) {
    throw std::logic_error("Null iterator");
  }
  return &curr_->val_;
}

template< class T >
bool kuznetsov::LRCIter< T >::operator==(const LRCIter& y) const noexcept
{
  return curr_ == y.curr_;
}

template< class T >
bool kuznetsov::LRCIter< T >::operator!=(const LRCIter& y) const noexcept
{
  return !(*this == y);
}
//-----------------------------------------------------
template< class T >
kuznetsov::LRIter< T >::LRIter(detail::Node< T >* pn):
  curr_(pn)
{}

template< class T >
kuznetsov::LRIter< T >& kuznetsov::LRIter< T >::operator++()
{
  if (curr_) {
    curr_ = curr_->prev_;
  }
  return *this;
}

template< class T >
kuznetsov::LRIter< T > kuznetsov::LRIter< T >::operator++(int)
{
  LRIter temp(*this);
  ++(*this);
  return temp;
}

template< class T >
kuznetsov::LRIter< T >& kuznetsov::LRIter< T >::operator--()
{
  if (curr_) {
    curr_ = curr_->next_;
  }
  return *this;
}

template< class T >
kuznetsov::LRIter< T > kuznetsov::LRIter< T >::operator--(int)
{
  LRIter temp(*this);
  --(*this);
  return temp;
}

template< class T >
T& kuznetsov::LRIter< T >::operator*()
{
  if (!this->curr_) {
    throw std::logic_error("Null iterator");
  }
  return curr_->val_;
}

template< class T >
T* kuznetsov::LRIter< T >::operator->()
{
  if (!this->curr_) {
    throw std::logic_error("Null iterator");
  }
  return &curr_->val_;
}

template< class T >
bool kuznetsov::LRIter< T >::operator==(const LRIter& y) const noexcept
{
  return curr_ == y.curr_;
}

template< class T >
bool kuznetsov::LRIter< T >::operator!=(const LRIter& y) const noexcept
{
  return !(*this == y);
}
//------------------------------
template< class T >
kuznetsov::List< T >::List():
  head_(nullptr),
  size_(0)
{}

template< class T >
kuznetsov::List< T >::List(const List& other):
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

template< class T >
kuznetsov::List< T >::List(List&& other) noexcept:
  head_(other.head_),
  size_(other.size_)
{
  other.head_ = nullptr;
  other.size_ = 0;
}

template< class T >
kuznetsov::List< T >::~List() noexcept
{
  clear();
}

template< class T >
kuznetsov::List< T >& kuznetsov::List< T >::operator=(const List& other)
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

template< class T >
kuznetsov::List< T >& kuznetsov::List< T >::operator=(List&& other) noexcept
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

template< class T >
kuznetsov::LIter< T > kuznetsov::List< T >::insert(LCIter< T > it, const T& val)
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

template< class T >
kuznetsov::LIter< T > kuznetsov::List< T >::insert(LCIter< T > it, T&& val)
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

template< class T >
T& kuznetsov::List< T >::front()
{
  if (!head_) {
    throw std::logic_error("Empty list");
  }
  return head_->val_;
}

template< class T >
T& kuznetsov::List< T >::back()
{
  if (!head_) {
    throw std::logic_error("Empty list");
  }
  return head_->prev_->val_;
}

template< class T >
const T& kuznetsov::List< T >::front() const
{
  if (!head_) {
    throw std::logic_error("Empty list");
  }
  return head_->val_;
}

template< class T >
const T& kuznetsov::List< T >::back() const
{
  if (!head_) {
    throw std::logic_error("Empty list");
  }
  return head_->prev_->val_;
}

template< class T >
void kuznetsov::List< T >::popFront()
{
  if (!head_) {
    throw std::logic_error("Empty list");
  }
  erase(cbegin());
}

template< class T >
void kuznetsov::List< T >::popBack()
{
  if (!head_) {
    throw std::logic_error("Empty list");
  }
  erase(LCIter< T >(head_->prev_));
}

template< class T >
kuznetsov::LIter< T > kuznetsov::List< T >::begin()
{
  return LIter< T >(head_);
}

template< class T >
kuznetsov::LIter< T > kuznetsov::List< T >::end()
{
  return LIter< T >(nullptr);
}

template< class T >
kuznetsov::LCIter< T > kuznetsov::List< T >::cbegin() const
{
  return LCIter< T >(head_);
}

template< class T >
kuznetsov::LCIter< T > kuznetsov::List< T >::cend() const
{
  return LCIter< T >(nullptr);
}

template< class T >
kuznetsov::LRIter< T > kuznetsov::List< T >::rbegin()
{
  return LRIter< T >(head_->prev_);
}

template< class T >
kuznetsov::LRIter< T > kuznetsov::List< T >::rend()
{
  return LRIter< T >(nullptr);
}

template< class T >
kuznetsov::LRCIter< T > kuznetsov::List< T >::rcbegin() const
{
  return LRCIter< T >(head_->prev_);
}

template< class T >
kuznetsov::LRCIter< T > kuznetsov::List< T >::rcend() const
{
  return LRCIter< T >(nullptr);
}

template< class T >
void kuznetsov::List< T >::clear()
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

template< class T >
kuznetsov::LIter< T > kuznetsov::List< T >::erase(LCIter< T > it)
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

template< class T >
size_t kuznetsov::List< T >::size() const
{
  return size_;
}

template< class T >
bool kuznetsov::List< T >::empty() const
{
  return !size_;
}


#endif

