#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <utility>
#include <cstddef>

namespace kuznetsov {
  namespace detail {
    template< class Key, class Value >
    struct Node {
      std::pair< const Key, Value > value_;

      Node< Key, Value >* parrent_;
      Node< Key, Value >* lt_;
      Node< Key, Value >* rt_;
    };
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

    void swap() noexcept;
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
  };
}

template< class Key, class Value, class Compare >
kuznetsov::BSTree< Key, Value, Compare >::BSTree():
  cmptr_(Compare{}),
  root_(nullptr),
  size_(0)
{}





template< class Key, class Value, class Compare >
kuznetsov::BSTree< Key, Value, Compare >::~BSTree()
{
  //TODO
}


template< class Key, class Value, class Compare >
size_t kuznetsov::BSTree< Key, Value, Compare >::getSize() const noexcept
{
  return size_;
}

template< class Key, class Value, class Compare >
size_t kuznetsov::BSTree< Key, Value, Compare >::height() const noexcept
{
  //TODO
  return 100;
}





#endif


