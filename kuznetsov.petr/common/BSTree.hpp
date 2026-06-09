#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace kuznetsov {
  namespace detail {
    template< class Key, class Value >
    struct Node {
      std::pair< const Key, Value > value_;

      Node* parent_;
      Node* lt_;
      Node* rt_;

      Node(const Key& k, const Value& v, Node* p);
    };

    template< class Key, class Value >
    Node< Key, Value >* copyTree(const Node< Key, Value >* oth, Node< Key, Value >* p = nullptr);

    template< class Key, class Value >
    Node< Key, Value >* minimum(Node< Key, Value >* root);

    template< class Key, class Value >
    Node< Key, Value >* maximum(Node< Key, Value >* root);

  }

  template< class Key, class Value, bool IsConst >
  struct BSTIterator;

  template< class Key, class Value, class Compare >
  struct BSTree {
    BSTree();
    BSTree(const BSTree&);
    BSTree(BSTree&&) noexcept;
    ~BSTree() noexcept;

    BSTree& operator=(const BSTree&);
    BSTree& operator=(BSTree&&) noexcept;

    using iterator = BSTIterator< Key, Value, false >;
    using const_iterator = BSTIterator< Key, Value, true >;

    template< class UV >
    void push(const Key& k, UV&& v);
    Value& at(const Key& k);
    const Value& at(const Key& k) const;
    void drop(const Key& k);

    const_iterator root() const;
    const_iterator leftChild(const_iterator) const;
    const_iterator rightChild(const_iterator) const;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);

    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it) const noexcept;
    size_t height() const noexcept;

    size_t getSize() const noexcept;
    bool isEmpty() const noexcept;
    bool contain(const Key& k) const noexcept;

    void swap(BSTree& oth) noexcept;
    void clear() noexcept;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;


  private:
    Compare cmptr_;
    detail::Node< Key, Value >* root_;
    size_t size_;

    detail::Node< Key, Value >* find(const Key& key) const noexcept;
    size_t calcHeight(const detail::Node< Key, Value >*) const noexcept;
    void clear(detail::Node< Key, Value >*) noexcept;
  };

  template< class Key, class Value, bool IsConst >
  struct BSTIterator {
    using type_t = std::pair< const Key, Value >;
    using reference = typename std::conditional< IsConst, const type_t&, type_t& >::type;
    using pointer = typename std::conditional< IsConst, const type_t*, type_t* >::type;

    BSTIterator& operator++();
    BSTIterator& operator--();

    BSTIterator operator++(int);
    BSTIterator operator--(int);

    reference operator*();
    pointer operator->();

    template< bool OthConst >
    bool operator==(const BSTIterator< Key, Value, OthConst >&);

    template< bool OthConst >
    bool operator!=(const BSTIterator< Key, Value, OthConst >&);

  private:
    template< class, class, class >
    friend struct BSTree;
    BSTIterator(detail::Node< Key, Value >*);
    detail::Node< Key, Value >* curr_;
  };
}

template< class K, class V, bool C >
template< bool OthConst >
bool kuznetsov::BSTIterator< K, V, C >::operator==(const BSTIterator< K, V, OthConst >& oth)
{
  return curr_ == oth.curr_;
}

template< class K, class V, bool C >
template< bool OthConst >
bool kuznetsov::BSTIterator< K, V, C >::operator!=(const BSTIterator< K, V, OthConst >& oth)
{
  return curr_ != oth.curr_;
}

template< class Key, class Value, bool IsConst >
typename kuznetsov::BSTIterator< Key, Value, IsConst >::reference
kuznetsov::BSTIterator< Key, Value, IsConst >::operator*()
{
  return curr_->value_;
}

template< class Key, class Value, bool IsConst >
typename kuznetsov::BSTIterator< Key, Value, IsConst >::pointer
kuznetsov::BSTIterator< Key, Value, IsConst >::operator->()
{
  return &curr_->value_;
}

template< class Key, class Value, bool IsConst >
kuznetsov::BSTIterator< Key, Value, IsConst >& kuznetsov::BSTIterator< Key, Value, IsConst >::operator++()
{
  detail::Node< Key, Value >* next = curr_;
  if (next->rt_) {
    next = next->rt_;
    next = detail::minimum(next);
  } else {
    detail::Node< Key, Value >* parent = next->parent_;
    while (parent && parent->lt_ != next) {
      next = parent;
      parent = next->parent_;
    }
    next = parent;
  }
  curr_ = next;
  return *this;
}

template< class Key, class Value, bool IsConst >
kuznetsov::BSTIterator< Key, Value, IsConst > kuznetsov::BSTIterator< Key, Value, IsConst >::operator++(int)
{
  auto it = BSTIterator< Key, Value, IsConst >(curr_);
  ++(*this);
  return it;
}

template< class Key, class Value, bool IsConst >
kuznetsov::BSTIterator< Key, Value, IsConst >& kuznetsov::BSTIterator< Key, Value, IsConst >::operator--()
{
  detail::Node< Key, Value >* next = curr_;
  if (next->lt_) {
    next = next->lt_;
    next = maximum(next);
  } else {
    detail::Node< Key, Value >* parent = next->parent_;
    while (parent && parent->rt_ != next) {
      next = parent;
      parent = next->parent_;
    }
    next = parent;
  }
  curr_ = next;
  return *this;
}

template< class Key, class Value, bool IsConst >
kuznetsov::BSTIterator< Key, Value, IsConst > kuznetsov::BSTIterator< Key, Value, IsConst >::operator--(int)
{
  auto it = BSTIterator< Key, Value, IsConst >(curr_);
  --(*this);
  return it;
}

template< class Key, class Value, bool IsConst >
kuznetsov::BSTIterator< Key, Value, IsConst >::BSTIterator(detail::Node< Key, Value >* n):
  curr_(n)
{}

template< class Key, class Value >
kuznetsov::detail::Node< Key, Value >::Node(const Key& k, const Value& v, Node* p):
  value_(std::make_pair(k, v)),
  parent_(p),
  lt_(nullptr),
  rt_(nullptr)
{}

template< class Key, class Value, class Compare >
kuznetsov::BSTree< Key, Value, Compare >::BSTree():
  cmptr_(Compare{}),
  root_(nullptr),
  size_(0)
{}

template< class Key, class Value, class Compare >
kuznetsov::BSTree< Key, Value, Compare >::BSTree(const BSTree& oth):
  cmptr_(oth.cmptr_),
  root_(nullptr),
  size_(oth.size_)
{
  root_ = detail::copyTree(oth.root_);
}

template< class Key, class Value, class Compare >
kuznetsov::BSTree< Key, Value, Compare >::BSTree(BSTree&& oth) noexcept:
  cmptr_(oth.cmptr_),
  root_(std::exchange(oth.root_, nullptr)),
  size_(std::exchange(oth.size_, 0))
{}

template< class Key, class Value >
kuznetsov::detail::Node< Key, Value >* kuznetsov::detail::copyTree(const Node< Key, Value >* oth, Node< Key, Value >* p)
{
  if (!oth) {
    return nullptr;
  }
  Node< Key, Value >* n = new Node< Key, Value >(oth->value_.first, oth->value_.second, p);
  try {
    n->lt_ = copyTree(oth->lt_, n);
    n->rt_ = copyTree(oth->rt_, n);
  } catch (...) {
    delete n;
    throw;
  }
  return n;
}

template< class Key, class Value, class Compare >
kuznetsov::BSTree< Key, Value, Compare >::~BSTree() noexcept
{
  clear();
}

template< class K, class V, class C >
kuznetsov::BSTree< K, V, C >& kuznetsov::BSTree< K, V, C >::operator=(const BSTree& oth)
{
  if (this == std::addressof(oth)) {
    return *this;
  }
  BSTree< K, V, C > cp(oth);
  swap(cp);
  return *this;
}

template< class K, class V, class C >
kuznetsov::BSTree< K, V, C >& kuznetsov::BSTree< K, V, C >::operator=(BSTree&& oth) noexcept
{
  if (this == std::addressof(oth)) {
    return *this;
  }
  BSTree< K, V, C > cp(std::move(oth));
  swap(cp);
  return *this;
}

template< class Key, class Value, class Compare >
template< class UV >
void kuznetsov::BSTree< Key, Value, Compare >::push(const Key& k, UV&& v)
{
  if (!root_) {
    root_ = new detail::Node< Key, Value >(k, std::forward< UV >(v), nullptr);
    ++size_;
    return;
  }

  detail::Node< Key, Value >* curr = find(k);
  detail::Node< Key, Value >* p;
  if (!curr) {
    curr = root_;
    while (curr) {
      p = curr;
      if (cmptr_(k, curr->value_.first)) {
        curr = curr->lt_;
      } else {
        curr = curr->rt_;
      }
    }
    if (cmptr_(k, p->value_.first)) {
      p->lt_ = new detail::Node< Key, Value >(k, std::forward< UV >(v), p);
    } else {
      p->rt_ = new detail::Node< Key, Value >(k, std::forward< UV >(v), p);
    }
    ++size_;
  } else {
    curr->value_.second = std::forward< UV >(v);
  }
}

template< class K, class V, class C >
const V& kuznetsov::BSTree< K, V, C >::at(const K& k) const
{
  detail::Node< K, V >* curr = find(k);
  if (!curr) {
    throw std::logic_error("No element with such case");
  }
  return curr->value_.second;
}

template< class K, class V, class C >
V& kuznetsov::BSTree< K, V, C >::at(const K& k)
{
  const BSTree< K, V, C >* cthis = this;
  return const_cast< V& >(cthis->at(k));
}
template< class K, class V, class Cmp >
void kuznetsov::BSTree< K, V, Cmp >::drop(const K& key)
{
  detail::Node< K, V >* curr = find(key);
  if (!curr) {
    throw std::logic_error("Element with such key doesnt exist");
  }

  if (curr->lt_ && curr->rt_) {
    detail::Node< K, V >* succ = detail::maximum(curr->lt_);

    if (succ->parent_ != curr) {
      succ->parent_->rt_ = succ->lt_;
      if (succ->lt_) {
        succ->lt_->parent_ = succ->parent_;
      }
      succ->lt_ = curr->lt_;
      curr->lt_->parent_ = succ;
    }
    succ->rt_ = curr->rt_;
    curr->rt_->parent_ = succ;
    succ->parent_ = curr->parent_;
    if (curr->parent_) {
      if (curr->parent_->lt_ == curr) {
        curr->parent_->lt_ = succ;
      } else {
        curr->parent_->rt_ = succ;
      }
    } else {
      root_ = succ;
    }
  } else {
    detail::Node< K, V >* child = curr->lt_ ? curr->lt_ : curr->rt_;
    if (curr->parent_) {
      if (curr->parent_->lt_ == curr) {
        curr->parent_->lt_ = child;
      } else {
        curr->parent_->rt_ = child;
      }
      if (child) {
        child->parent_ = curr->parent_;
      }
    } else {
      root_ = child;
      if (child) {
        child->parent_ = nullptr;
      }
    }
  }
  delete curr;
  --size_;
}

template< class K, class V, class Cmp >
kuznetsov::detail::Node< K, V >* kuznetsov::BSTree< K, V, Cmp >::find(const K& key) const noexcept
{
  detail::Node< K, V >* curr = root_;
  while (curr) {
    if (cmptr_(key, curr->value_.first)) {
      curr = curr->lt_;
    } else if (cmptr_(curr->value_.first, key)) {
      curr = curr->rt_;
    } else {
      return curr;
    }
  }
  return nullptr;
}

template< class Key, class Value, class Compare >
bool kuznetsov::BSTree< Key, Value, Compare >::contain(const Key& k) const noexcept
{
  detail::Node< Key, Value >* curr = find(k);
  return curr != nullptr;
}

template< class Key, class Value, class Compare >
size_t kuznetsov::BSTree< Key, Value, Compare >::getSize() const noexcept
{
  return size_;
}

template< class Key, class Value, class Compare >
bool kuznetsov::BSTree< Key, Value, Compare >::isEmpty() const noexcept
{
  return !size_;
}

template< class Key, class Value, class Compare >
size_t kuznetsov::BSTree< Key, Value, Compare >::height() const noexcept
{
  return calcHeight(root_);
}

template< class Key, class Value, class Compare >
size_t kuznetsov::BSTree< Key, Value, Compare >::height(const_iterator it) const noexcept
{
  return calcHeight(it.curr_);
}

template< class Key, class Value, class Compare >
size_t kuznetsov::BSTree< Key, Value, Compare >::calcHeight(const detail::Node< Key, Value >* n) const noexcept
{
  if (!n) {
    return 0;
  }
  return 1 + std::max(calcHeight(n->lt_), calcHeight(n->rt_));
}

template< class Key, class Value, class Compare >
void kuznetsov::BSTree< Key, Value, Compare >::swap(BSTree& oth) noexcept
{
  std::swap(root_, oth.root_);
  std::swap(size_, oth.size_);
}

template< class Key, class Value, class Compare >
void kuznetsov::BSTree< Key, Value, Compare >::clear(detail::Node< Key, Value >* node) noexcept
{
  if (!node) {
    return;
  }
  clear(node->lt_);
  clear(node->rt_);
  delete node;
}

template< class Key, class Value, class Compare >
void kuznetsov::BSTree< Key, Value, Compare >::clear() noexcept
{
  clear(root_);
  root_ = nullptr;
  size_ = 0;
}

template< class Key, class Value >
kuznetsov::detail::Node< Key, Value >* kuznetsov::detail::minimum(Node< Key, Value >* root)
{
  if (!root) {
    return nullptr;
  }
  auto curr = root;
  while (curr->lt_) {
    curr = curr->lt_;
  }
  return curr;
}

template< class Key, class Value >
kuznetsov::detail::Node< Key, Value >* kuznetsov::detail::maximum(Node< Key, Value >* root)
{
  if (!root) {
    return nullptr;
  }
  auto curr = root;
  while (curr->rt_) {
    curr = curr->rt_;
  }
  return curr;
}

template< class K, class V, class Cmp >
typename kuznetsov::BSTree< K, V, Cmp >::const_iterator kuznetsov::BSTree< K, V, Cmp >::rotateLeft(const_iterator it)
{
  detail::Node< K, V >* y = it.curr_;

  if (!y || !y->parent_) {
    return const_iterator(y);
  }

  detail::Node< K, V >* x = y->parent_;

  if (x->rt_ != y) {
    throw std::logic_error("Invalid left rotation");
  }

  x->rt_ = y->lt_;
  if (y->lt_) {
    y->lt_->parent_ = x;
  }

  y->parent_ = x->parent_;

  if (!x->parent_) {
    root_ = y;
  } else if (x->parent_->lt_ == x) {
    x->parent_->lt_ = y;
  } else {
    x->parent_->rt_ = y;
  }

  y->lt_ = x;
  x->parent_ = y;

  return const_iterator(y);
}

template< class K, class V, class Cmp >
typename kuznetsov::BSTree< K, V, Cmp >::const_iterator kuznetsov::BSTree< K, V, Cmp >::rotateRight(const_iterator it)
{
  detail::Node< K, V >* x = it.curr_;

  if (!x || !x->parent_) {
    return const_iterator(x);
  }

  detail::Node< K, V >* y = x->parent_;

  if (y->lt_ != x) {
    throw std::logic_error("Invalid right rotation");
  }

  y->lt_ = x->rt_;
  if (x->rt_) {
    x->rt_->parent_ = y;
  }

  x->parent_ = y->parent_;

  if (!y->parent_) {
    root_ = x;
  } else if (y->parent_->lt_ == y) {
    y->parent_->lt_ = x;
  } else {
    y->parent_->rt_ = x;
  }

  x->rt_ = y;
  y->parent_ = x;

  return const_iterator(x);
}

template< class K, class V, class Cmp >
typename kuznetsov::BSTree< K, V, Cmp >::const_iterator
kuznetsov::BSTree< K, V, Cmp >::rotateLargeLeft(const_iterator it)
{
  detail::Node< K, V >* b = it.curr_;

  if (!b || !b->parent_ || !b->parent_->parent_) {
    throw std::logic_error("Invalid large left rotation");
  }
  detail::Node< K, V >* c = b->parent_;
  detail::Node< K, V >* a = c->parent_;
  if (a->rt_ != c || c->lt_ != b) {
    throw std::logic_error("Not a large left rotation configuration");
  }
  rotateRight(const_iterator(b));
  return rotateLeft(const_iterator(b));
}

template< class K, class V, class Cmp >
typename kuznetsov::BSTree< K, V, Cmp >::const_iterator
kuznetsov::BSTree< K, V, Cmp >::rotateLargeRight(const_iterator it)
{
  detail::Node< K, V >* b = it.curr_;

  if (!b || !b->parent_ || !b->parent_->parent_) {
    throw std::logic_error("Invalid large right rotation");
  }

  detail::Node< K, V >* a = b->parent_;
  detail::Node< K, V >* c = a->parent_;

  if (c->lt_ != a || a->rt_ != b) {
    throw std::logic_error("Not a large right rotation configuration");
  }

  rotateLeft(const_iterator(b));
  return rotateRight(const_iterator(b));
}

template< class K, class V, class C >
kuznetsov::BSTIterator< K, V, false > kuznetsov::BSTree< K, V, C >::begin()
{
  return BSTIterator< K, V, false >(detail::minimum(root_));
}

template< class K, class V, class C >
kuznetsov::BSTIterator< K, V, true > kuznetsov::BSTree< K, V, C >::begin() const
{
  return BSTIterator< K, V, true >(detail::minimum(root_));
}

template< class K, class V, class C >
kuznetsov::BSTIterator< K, V, true > kuznetsov::BSTree< K, V, C >::cbegin() const
{
  return BSTIterator< K, V, true >(detail::minimum(root_));
}

template< class K, class V, class C >
kuznetsov::BSTIterator< K, V, false > kuznetsov::BSTree< K, V, C >::end()
{
  return BSTIterator< K, V, false >(nullptr);
}

template< class K, class V, class C >
kuznetsov::BSTIterator< K, V, true > kuznetsov::BSTree< K, V, C >::end() const
{
  return BSTIterator< K, V, true >(nullptr);
}

template< class K, class V, class C >
kuznetsov::BSTIterator< K, V, true > kuznetsov::BSTree< K, V, C >::cend() const
{
  return BSTIterator< K, V, true >(nullptr);
}

template< class Key, class Value, class Compare >
typename kuznetsov::BSTree< Key, Value, Compare >::const_iterator kuznetsov::BSTree< Key, Value, Compare >::root() const
{
  return const_iterator(root_);
}
template< class Key, class Value, class Compare >
typename kuznetsov::BSTree< Key, Value, Compare >::const_iterator
kuznetsov::BSTree< Key, Value, Compare >::leftChild(const_iterator it) const
{
  return const_iterator(it.curr_->lt_);
}
template< class Key, class Value, class Compare >
typename kuznetsov::BSTree< Key, Value, Compare >::const_iterator
kuznetsov::BSTree< Key, Value, Compare >::rightChild(const_iterator it) const
{
  return const_iterator(it.curr_->rt_);
}

#endif
