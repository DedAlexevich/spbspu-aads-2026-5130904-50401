#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <stdexcept>
#include <cstddef>
#include <utility>
#include <cmath>

namespace kuznetsov {

  enum class State {
    FREE, STORE, DELETED
  };

  template< bool B, class T, class F >
  struct conditional {
    using type = T;
  };

  template<class T, class F>
  struct conditional< false, T, F > {
    using type = F;
  };

  template< class Key, class Value, bool IsConst >
  struct Iterator;

  template< class F, class S >
  struct Slot {
    Slot() = delete;
    Slot(F&& f, S&& s):
      first(std::move(f)),
      second(std::move(s))
    {}
    Slot(const F& f, const S& s):
      first(f),
      second(s)
    {}


    F first;
    S second;
  };

  template< class Key, class Value, class Hash, class Equal >
  struct HashTable {
    using const_iterator = Iterator< Key, Value, true >;
    using iterator = Iterator< Key, Value, false >;

    HashTable();
    HashTable(const HashTable&);
    HashTable(HashTable&&) noexcept;
    HashTable(size_t capacity);

    ~HashTable();

    HashTable& operator=(const HashTable&);
    HashTable& operator=(HashTable&&) noexcept;

    void add(Key k, Value val);
    void remove(Key k);
    bool has(Key k) const;
    void rehash(size_t slots = 0);

    void swap(HashTable&) noexcept;

    size_t getSize() const;
    size_t getCapacity() const;

    Value& at(Key k);
    const Value& at(Key k) const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
  private:
    Hash hasher_;
    Equal comparator_;
    State* states_;
    Slot< Key, Value >* slots_;
    size_t size_;
    size_t capacity_;
  };

  template< class Key, class Value, bool IsConst >
  struct Iterator {
    using pair_type = Slot< const Key, Value >;
    using reference = typename conditional< IsConst, const pair_type&, pair_type& >::type;
    using point = typename conditional< IsConst, const pair_type*, pair_type* >::type;

    Iterator(Slot< Key, Value >* slots , State* s, size_t ind, size_t cap);

    template< bool OthConst >
    bool operator==(const Iterator< Key, Value, OthConst >&) const;

    template< bool OthConst >
    bool operator!=(const Iterator< Key, Value, OthConst >&) const;

    reference operator*();
    point operator->();

    Iterator operator++();
    Iterator operator--();

    Iterator operator++(int);
    Iterator operator--(int);

  private:
    pair_type* slots_;
    State* states_;
    size_t i_;
    size_t cap_;
  };
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::HashTable< Key, Value, Hash, Equal >::iterator
kuznetsov::HashTable< Key, Value, Hash, Equal >::begin()
{
  size_t i = 0;
  while (i < capacity_ && states_[i] != State::STORE) {
    ++i;
  }
  return iterator(slots_, states_, i, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::HashTable< Key, Value, Hash, Equal >::const_iterator
kuznetsov::HashTable< Key, Value, Hash, Equal >::begin() const
{
  size_t i = 0;
  while (i < capacity_ && states_[i] != State::STORE) {
    ++i;
  }
  return const_iterator(slots_, states_, i, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::HashTable< Key, Value, Hash, Equal >::const_iterator
kuznetsov::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  size_t i = 0;
  while (i < capacity_ && states_[i] != State::STORE) {
    ++i;
  }
  return const_iterator(slots_, states_, i, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::HashTable< Key, Value, Hash, Equal >::iterator
kuznetsov::HashTable< Key, Value, Hash, Equal >::end()
{
  return iterator(slots_, states_, capacity_, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::HashTable< Key, Value, Hash, Equal >::const_iterator
kuznetsov::HashTable< Key, Value, Hash, Equal >::end() const
{
  return const_iterator(slots_, states_, capacity_, capacity_);
}

template< class Key, class Value, class Hash, class Equal >
typename kuznetsov::HashTable< Key, Value, Hash, Equal >::const_iterator
kuznetsov::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return const_iterator(slots_, states_, capacity_, capacity_);
}


template< class Key, class Value, bool IsConst >
kuznetsov::Iterator< Key, Value, IsConst >::Iterator(Slot< Key, Value >* slt, State* s, size_t ind, size_t cap):
  slots_(reinterpret_cast< pair_type* >(slt)),
  states_(s),
  i_(ind),
  cap_(cap)
{}

template< class Key, class Value, bool IsConst >
template< bool OthConst >
bool kuznetsov::Iterator< Key, Value, IsConst >::operator==(const Iterator< Key, Value, OthConst >& oth) const
{
  bool f = (this->slots_ + this->i_) == (oth.slots_ + oth.i_);
  return f;
}

template< class Key, class Value, bool IsConst >
template< bool OthConst >
bool kuznetsov::Iterator< Key, Value, IsConst >::operator!=(const Iterator< Key, Value, OthConst >& oth) const
{
  return !(*this == oth);
}

template< class Key, class Value, bool IsConst >
typename kuznetsov::Iterator< Key, Value, IsConst >::reference
kuznetsov::Iterator< Key, Value, IsConst >::operator*()
{
  return slots_[i_];
}

template< class Key, class Value, bool IsConst >
typename kuznetsov::Iterator< Key, Value, IsConst >::point
kuznetsov::Iterator< Key, Value, IsConst >::operator->()
{
  return slots_ + i_;
}

template< class Key, class Value, bool IsConst >
kuznetsov::Iterator< Key, Value, IsConst > kuznetsov::Iterator< Key, Value, IsConst >::operator++()
{
  ++i_;
  while (i_ < cap_ && states_[i_] != State::STORE) {
    ++i_;
  }
  return *this;
}

template< class Key, class Value, bool IsConst >
kuznetsov::Iterator< Key, Value, IsConst > kuznetsov::Iterator< Key, Value, IsConst >::operator++(int)
{
  Iterator tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class Value, bool IsConst >
kuznetsov::Iterator< Key, Value, IsConst > kuznetsov::Iterator< Key, Value, IsConst >::operator--()
{
  if (i_ == 0) {
    return *this;
  }
  while (i_ > 0 && states_[i_] != State::STORE) {
    --i_;
  }
  return *this;
}

template< class Key, class Value, bool IsConst >
kuznetsov::Iterator< Key, Value, IsConst > kuznetsov::Iterator< Key, Value, IsConst >::operator--(int)
{
  Iterator tmp = *this;
  --(*this);
  return tmp;
}

template< class Key, class Value, class Hash, class Equal >
kuznetsov::HashTable< Key, Value, Hash, Equal >::HashTable():
  HashTable(16)
{}

template< class Key, class Value, class Hash, class Equal >
kuznetsov::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  for (size_t i = 0; i < capacity_; ++i) {
    if (states_[i] == State::STORE) {
      (slots_ + i)->~Slot();
    }
  }
  ::operator delete(slots_);
  delete[] states_;
}

template< class Key, class Value, class Hash, class Equal >
size_t kuznetsov::HashTable< Key, Value, Hash, Equal >::getSize() const
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t kuznetsov::HashTable< Key, Value, Hash, Equal >::getCapacity() const
{
  return capacity_;
}

template< class Key, class Value, class Hash, class Equal >
kuznetsov::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& oth):
  HashTable(oth.getCapacity())
{
  for (size_t i = 0; i < capacity_; ++i) {
    if (oth.states_[i] == State::STORE) {
      new (slots_ + i) Slot< Key, Value >(oth.slots_[i].first, oth.slots_[i].second);
      ++size_;
    }
    states_[i] = oth.states_[i];
  }
}


template< class Key, class Value, class Hash, class Equal >
kuznetsov::HashTable< Key, Value, Hash, Equal >::HashTable(size_t capacity):
  hasher_(Hash{}),
  comparator_(Equal{}),
  states_(nullptr),
  slots_(nullptr),
  size_(0),
  capacity_(std::pow(2,ceil(log2(capacity))))
{
  try {
    states_ = new State[capacity_] {};
    slots_ = static_cast< Slot< Key, Value >* >(::operator new(sizeof(Slot< Key, Value >) * capacity_));
  } catch (...) {
    delete[] states_;
    ::operator delete(slots_);
    throw;
  }
}

template< class Key, class Value, class Hash, class Equal >
kuznetsov::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& oth) noexcept :
  hasher_(oth.hasher_),
  comparator_(oth.comparator_),
  states_(oth.states_),
  slots_(oth.slots),
  size_(oth.size_),
  capacity_(oth.capacity_)
{
  oth.states_ = nullptr;
  oth.slots_ = nullptr;
  oth.size_ = 0;
  oth.capacity_ = 0;
}

template< class Key, class Value, class Hash, class Equal >
kuznetsov::HashTable< Key, Value, Hash, Equal >&
kuznetsov::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& oth)
{
  if(std::addressof(oth) == this) {
    return *this;
  }
  HashTable< Key, Value, Hash, Equal > cp(oth);
  swap(cp);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
kuznetsov::HashTable< Key, Value, Hash, Equal >&
kuznetsov::HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& oth) noexcept
{
  if(std::addressof(oth) == this) {
    return *this;
  }
  HashTable< Key, Value, Hash, Equal > cp(std::move(oth));
  swap(cp);
  return *this;
}

template< class Key, class Value, class Hash, class Equal >
void kuznetsov::HashTable< Key, Value, Hash, Equal >::swap(HashTable& oth) noexcept
{
  std::swap(hasher_, oth.hasher_);
  std::swap(comparator_, oth.comparator_);
  std::swap(states_, oth.states_);
  std::swap(slots_, oth.slots_);
  std::swap(size_, oth.size_);
  std::swap(capacity_, oth.capacity_);
}

template< class Key, class Value, class Hash, class Equal >
void kuznetsov::HashTable< Key, Value, Hash, Equal >::add(Key k, Value val)
{
  if (size_ == capacity_) {
    throw std::logic_error("Not enough slots_");
  }
  if (has(k)) {
    throw std::logic_error("Such key already exist");
  }
  size_t hash = hasher_(k);
  size_t pos = 0;
  size_t i = 0;
  for (; i < capacity_; ++i) {
    pos = (hash + (i + i * i) / 2) % capacity_;
    if (states_[pos] == State::FREE or states_[pos] == State::DELETED) {
      break;
    }
  }
  new (slots_ + pos) Slot< Key, Value >(k, val);
  states_[pos] = State::STORE;
  ++size_;
}

template< class Key, class Value, class Hash, class Equal >
bool kuznetsov::HashTable< Key, Value, Hash, Equal >::has(Key k) const
{
  size_t hash = hasher_(k);
  size_t i = 0;
  size_t pos = 0;
  for (; i < capacity_; ++i) {
    pos = (hash + (i + i * i) / 2) % capacity_;
    if (states_[pos] == State::FREE) {
      return false;
    }
    if (states_[pos] == State::STORE) {
      if (comparator_(k, slots_[pos].first)) {
        return true;
      }
    }
  }
  return false;
}

template< class Key, class Value, class Hash, class Equal >
void kuznetsov::HashTable< Key, Value, Hash, Equal >::remove(Key k)
{
  size_t hash = hasher_(k);
  size_t i = 0;
  size_t pos = 0;
  for (; i < capacity_; ++i) {
    pos = (hash + (i + i * i) / 2) % capacity_;
    if (states_[pos] == State::FREE) {
      throw std::logic_error("Not found key");
    }
    if (states_[pos] == State::STORE) {
      if (comparator_(k, slots_[pos].first)) {
        (slots_ + pos)->~Slot();
        states_[pos] = State::DELETED;
        --size_;
        return;
      }
    }
  }
  throw std::logic_error("Unexpected error");
}

template< class Key, class Value, class Hash, class Equal >
void kuznetsov::HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{

  size_t newCap = !slots ? capacity_ * 2 : pow(2, ceil(log2(slots)));

  HashTable< Key, Value, Hash, Equal > newTable(newCap);
  for (size_t i = 0; i < capacity_; ++i) {
    if (states_[i] == State::STORE) {
      newTable.add(slots_[i].first, slots_[i].second);
    }
  }
  swap(newTable);
}

template< class Key, class Value, class Hash, class Equal >
const Value& kuznetsov::HashTable< Key, Value, Hash, Equal >::at(Key k) const
{
  size_t hash = hasher_(k);
  size_t pos = 0;
  for (size_t i = 0; i < capacity_; ++i) {
    pos = (hash + (i + i * i) / 2) % capacity_;
    if (states_[pos] == State::FREE) {
      break;
    }
    if (states_[pos] == State::STORE && comparator_(k, slots_[pos].first)) {
      return slots_[pos].second;
    }
  }
  throw std::out_of_range("Key not found");
}

template< class Key, class Value, class Hash, class Equal >
Value& kuznetsov::HashTable< Key, Value, Hash, Equal >::at(Key k)
{
  const HashTable* cthis = this;
  return const_cast< Value& >((*cthis).at(k));
}

#endif

