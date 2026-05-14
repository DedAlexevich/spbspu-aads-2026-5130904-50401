#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <utility>
#include <cstddef>

namespace kuznetsov {

  template< class Key, class Value >
  struct Node {
    std::pair< const Key, Value > t_;

    Node< Key, Value >* parrent_;
    Node< Key, Value >* lt_;
    Node< Key, Value >* rt_;
  };

  template< class Key, class Value, bool IsConst>
  struct Iterator;

  template< class Key, class Value, class Compare >
  struct BSTree {

    BSTree();
    BSTree(const BSTree&);
    BSTree(BSTree&&);
    BSTree(std::initializer_list< std::pair< Key, Value > > init);
    ~BSTree();

    BSTree& operator=(const BSTree&);
    BSTree& operator=(BSTree&&);

    using iterator = Iterator< Key, Value, false >;
    using const_iterator = Iterator< Key, Value, true >;

    void push(Key k, Value v);
    Value get(Key k);
    void drop(Key k);

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);


    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it);
    size_t height();

    size_t getSize() const noexcept;
    bool isEmpty() const noexcept;
    bool contain(Key k) const noexcept;

    void swap();
    void clear();

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;


  private:
    Node< Key, Value >* root_;
    size_t size_;
  };

}

#endif


