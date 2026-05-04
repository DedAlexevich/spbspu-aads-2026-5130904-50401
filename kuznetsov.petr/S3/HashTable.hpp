#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

namespace kuznetsov {
  
  enum class State {
    FREE, STORE, DELETED
  };

  template< class Key, class Value, class Hash, class Equal >
  struct HashTable {

    HashTable();
    HashTable(const HashTable&);
    HashTable(HashTable&&) noexcept;
    HashTable(size_t capacity);

    ~HashTable();

    HashTable& operator=(const HashTable&);
    HashTable& operator=(HashTable&&) noexcept;
    
    void add(Key k, Value val);
    Value drop(Key k);
    bool has(Key k);
    void rehash(size_t slots);
    
    void extend();
    void swap(HashTable&) noexcept;
    
    size_t getSize();
    size_t getCapacity();

    Value& at(Key k);
    const Value& at(Key k) const;

    Value& operator[](Key k);
    const Value& operator[](Key k) const;

  private:
    Hash hasher_;
    Equal comparator_;
    State* states_;
    Value* values_;
    size_t size_;
    size_t capacity_;
  }; 
}


template< class Key, class Value, class Hash, class Equal >
kuznetsov::HashTable< Key, Value, Hash, Equal >::HashTable():
  hasher_(Hash{}),
  comparator_(Equal{}),
  states_(new State[10]{}),
  values_(static_cast< Value* >(::operator new(sizeof(Value) * 10))),
  size_(0),
  capacity_(10)
{}

template< class Key, class Value, class Hash, class Equal >
kuznetsov::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  for (size_t i = 0; i < capacity_; ++i) {
    if (states_[i] == State::STORE) {
      (values_ + i)->~Value();
    }
  }
  ::operator delete(values_);
  delete[] states_;
}

template< class Key, class Value, class Hash, class Equal >
size_t kuznetsov::HashTable< Key, Value, Hash, Equal >::getSize()
{
  return size_;
}

template< class Key, class Value, class Hash, class Equal >
size_t kuznetsov::HashTable< Key, Value, Hash, Equal >::getCapacity()
{
  return capacity_;
}


#endif

