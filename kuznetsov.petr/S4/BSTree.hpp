#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <utility>
#include <algorithm>
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
    Node< Key, Value >* copyTree(Node< Key, Value >*, Node< Key, Value >*);
  }

  template< class Key, class Value, bool IsConst>
  struct Iterator;

  template< class Key, class Value, class Compare >
  struct BSTree {

    BSTree();
    BSTree(const BSTree&);
    BSTree(BSTree&&) noexcept;
    BSTree(std::initializer_list< std::pair< Key, Value > > init);
    ~BSTree();

    BSTree& operator=(const BSTree&);
    BSTree& operator=(BSTree&&) noexcept;

    using iterator = Iterator< Key, Value, false >;
    using const_iterator = Iterator< Key, Value, true >;

    void push(Key k, Value v);
    Value& at(Key k);
    const Value& at(Key k) const;
    void drop(Key k);

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);


    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it);
    size_t height() const noexcept;

    size_t getSize() const noexcept;
    bool isEmpty() const noexcept;
    bool contain(Key k) const noexcept;

    void swap(BSTree& oth) noexcept;
    void clear();

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

    size_t calcHeight(detail::Node< Key, Value >*) const noexcept;
    void clear(detail::Node< Key, Value >*) noexcept;
  };
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

template< class Key, class Value >
kuznetsov::detail::Node< Key, Value >* kuznetsov::detail::copyTree(Node< Key, Value >* oth,
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
kuznetsov::BSTree< Key, Value, Compare >::~BSTree()
{
  clear();
}


template< class Key, class Value, class Compare >
size_t kuznetsov::BSTree< Key, Value, Compare >::getSize() const noexcept
{
  return size_;
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
void kuznetsov::BSTree< Key, Value, Compare>::clear()
{
  clear(root_);
}

#endif


