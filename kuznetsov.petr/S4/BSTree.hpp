#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <cstddef>

namespace kuznetsov {
  namespace detail {
    template< class Key, class Value >
    struct Node {
      std::pair< const Key, Value > value_;

      Node< Key, Value >* parent_;
      Node< Key, Value >* lt_;
      Node< Key, Value >* rt_;

      Node(const Key& k, const Value& v, Node* p);
    };

    template< class Key, class Value >
    Node< Key, Value >* copyTree(const Node< Key, Value >* oth, Node< Key, Value >* p = nullptr);

    template< class Key, class Value >
    Node< Key, Value >* rightMin(Node< Key, Value >* root);

    template< class Key, class Value >
    Node< Key, Value >* leftMax(Node< Key, Value >* root);

  }

  template< class Key, class Value, bool IsConst>
  struct Iterator;

  template< class Key, class Value, class Compare >
  struct BSTree {

    BSTree();
    BSTree(const BSTree&);
    BSTree(BSTree&&) noexcept;
    ~BSTree() noexcept;

    BSTree& operator=(const BSTree&);
    BSTree& operator=(BSTree&&) noexcept;

    using iterator = Iterator< Key, Value, false >;
    using const_iterator = Iterator< Key, Value, true >;

    template< class UV >
    void push(const Key& k, UV&& v);
    Value& at(const Key& k);
    const Value& at(const Key& k) const;
    void drop(const Key& k);

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
    size_t calcHeight(detail::Node< Key, Value >*) const noexcept;
    void clear(detail::Node< Key, Value >*) noexcept;
  };

  template< class Key, class Value, bool IsConst>
  struct Iterator {
    using type_t = detail::Node< Key, Value >;
    using reference = typename std::conditional< IsConst, const type_t&, type_t& >::type;
    using pointer = typename std::conditional< IsConst, const type_t*, type_t* >::type;

    Iterator& operator++();
    Iterator& operator--();

    Iterator operator++(int);
    Iterator operator--(int);

    reference operator*();
    pointer operator->();

    template< bool OthConst >
    bool operator==(const Iterator< Key, Value, OthConst >&);

    template< bool OthConst >
    bool operator!=(const Iterator< Key, Value, OthConst >&);

  private:
    Iterator(detail::Node< Key, Value >*);
    detail::Node< Key, Value >* curr_;
  };
}

template< class K, class V, bool C >
template< bool OthConst >
bool kuznetsov::Iterator< K, V, C >::operator==(const Iterator< K, V, OthConst >& oth)
{
  return curr_ == std::addressof(oth);
}

template< class K, class V, bool C >
template< bool OthConst >
bool kuznetsov::Iterator< K, V, C >::operator!=(const Iterator< K, V, OthConst >& oth)
{
  return curr_ != std::addressof(oth);
}

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
kuznetsov::detail::Node< Key, Value >* kuznetsov::detail::copyTree(const Node< Key, Value >* oth,
                                                                    Node< Key, Value >* p)
{
  if (!oth) {
    return nullptr;
  }
  Node< Key, Value >* n = new Node< Key, Value >(oth->value_.first, oth->value_.second, p);
  try {
    n->lt_ = copyTree(oth->lt_, n);
    n->rt_ = copyTree(oth->rt_, n);
  } catch(...) {
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
  if (contain(k)) {
    throw std::logic_error("Element with such key already exist");
  }

  if (!root_) {
    root_ = new detail::Node< Key, Value >(k, std::forward< UV >(v), nullptr);
    ++size_;
    return;
  }

  detail::Node< Key, Value >* curr = root_;
  detail::Node< Key, Value >* p;
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
    detail::Node< K, V >* succ = detail::leftMax(curr);

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
size_t kuznetsov::BSTree< Key, Value, Compare >::calcHeight(detail::Node< Key, Value >* n) const noexcept
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
void kuznetsov::BSTree< Key, Value, Compare>::clear(detail::Node< Key, Value >* node) noexcept
{
  if(!node) {
    return;
  }
  clear(node->lt_);
  clear(node->rt_);
  delete node;
}

template< class Key, class Value, class Compare >
void kuznetsov::BSTree< Key, Value, Compare>::clear() noexcept
{
  clear(root_);
}


template< class Key, class Value >
kuznetsov::detail::Node< Key, Value >* kuznetsov::detail::rightMin(Node< Key, Value >* root)
{
  auto curr = root->rt_;
  while (curr->lt_) {
    curr = curr->lt_;
  }
  return curr;
}

template< class Key, class Value >
kuznetsov::detail::Node< Key, Value >* kuznetsov::detail::leftMax(Node< Key, Value >* root)
{
  auto curr = root->lt_;
  while (curr->rt_) {
    curr = curr->rt_;
  }
  return curr;
}

#endif


