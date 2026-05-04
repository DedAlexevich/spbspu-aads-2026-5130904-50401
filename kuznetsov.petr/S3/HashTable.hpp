#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

namespace kuznetsov {
  
  enum class State {
    FREE, STORE, DELETED
  };

  template< class K, class Value, class Hash, class Equal >
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
    Hash hasher;
    Equal comparator;
    State* states;
    Value* values;
    size_t size;
    size_t capacity;
 }; 




}
#endif

