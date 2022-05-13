//---------------------------------------------------------------------------
// NAME: Joey Macauley
// FILE: hashmap.h
// DATE: CPSC 223 - Spring 2022
// DESC: Implementation of a HashMap using arrayseq
//---------------------------------------------------------------------------

#ifndef HASHMAP_H
#define HASHMAP_H

#include "map.h"
#include "arrayseq.h"
#include <functional>

template <typename K, typename V>
class HashMap : public Map<K, V>
{
public:
  // default constructor
  HashMap();

  // copy constructor
  HashMap(const HashMap &rhs);

  // move constructor
  HashMap(HashMap &&rhs);

  // copy assignment
  HashMap &operator=(const HashMap &rhs);

  // move assignment
  HashMap &operator=(HashMap &&rhs);

  // destructor
  ~HashMap();

  // Returns the number of key-value pairs in the map
  int size() const;

  // Tests if the map is empty
  bool empty() const;

  // Allows values associated with a key to be updated. Throws
  // out_of_range if the given key is not in the collection.
  V &operator[](const K &key);

  // Returns the value for a given key. Throws out_of_range if the
  // given key is not in the collection.
  const V &operator[](const K &key) const;

  // Extends the collection by adding the given key-value pair.
  // Expects key to not exist in map prior to insertion.
  void insert(const K &key, const V &value);

  // Shrinks the collection by removing the key-value pair with the
  // given key. Does not modify the collection if the collection does
  // not contain the key. Throws out_of_range if the given key is not
  // in the collection.
  void erase(const K &key);

  // Returns true if the key is in the collection, and false otherwise.
  bool contains(const K &key) const;

  // Returns the keys k in the collection such that k1 <= k <= k2
  ArraySeq<K> find_keys(const K &k1, const K &k2) const;

  // Returns the keys in the collection in ascending sorted order
  ArraySeq<K> sorted_keys() const;

  // Gives the key (as an ouptput parameter) immediately after the
  // given key according to ascending sort order. Returns true if a
  // successor key exists, and false otherwise.
  bool next_key(const K &key, K &next_key) const;

  // Gives the key (as an ouptput parameter) immediately before the
  // given key according to ascending sort order. Returns true if a
  // predecessor key exists, and false otherwise.
  bool prev_key(const K &key, K &next_key) const;

  // Removes all key-value pairs from the map. Does not change the
  // current capacity of the table.
  void clear();

  // statistics functions for the hash table implementation
  int min_chain_length() const;
  int max_chain_length() const;
  double avg_chain_length() const;

private:
  // node for linked-list separate chaining
  struct Node
  {
    K key;
    V value;
    Node *next;
  };

  // number of key-value pairs in map
  int count = 0;

  // max size of the (array) table
  int capacity = 16;

  // threshold for resize and rehash
  const double load_factor_threshold = 0.75;

  // array of linked lists
  Node **table = new Node *[capacity];

  // the hash function
  int hash(const K &key) const;

  // resize and rehash the table
  void resize_and_rehash();

  // initialize the table to all nullptr
  void init_table();
};

template <typename K, typename V>
HashMap<K, V>::HashMap()
{
  init_table();
}

// copy constructor
template <typename K, typename V>
HashMap<K, V>::HashMap(const HashMap &rhs)
{
  init_table();
  *this = rhs;
}

// move constructor
template <typename K, typename V>
HashMap<K, V>::HashMap(HashMap &&rhs)
{
  init_table();
  *this = std::move(rhs);
}

// copy assignment
template <typename K, typename V>
HashMap<K, V> &HashMap<K, V>::operator=(const HashMap &rhs)
{
  if (this != &rhs)
  {
    clear();
    Node *rhsTraverse = nullptr;
    Node *temp = nullptr;

    count = rhs.count;
    capacity = rhs.capacity;
    table = new Node *[capacity];
    init_table();

    for (int i = 0; i < capacity; i++)
    {
      if (rhs.table[i] != nullptr)
      {
        rhsTraverse = rhs.table[i];
        while (rhsTraverse != nullptr)
        {
          // new node
          temp = new Node;
          temp->key = rhsTraverse->key;
          temp->value = rhsTraverse->value;
          temp->next = table[i];
          table[i] = temp;

          // next
          rhsTraverse = rhsTraverse->next;
        }
      }
    }
  }
  return *this;
}

// move assignment
template <typename K, typename V>
HashMap<K, V> &HashMap<K, V>::operator=(HashMap &&rhs)
{
  if (this != &rhs)
  {
    count = rhs.count;
    capacity = rhs.capacity;

    for (int i = 0; i < capacity; ++i)
    {
      if (rhs.table[i] != nullptr)
      {
        table[i] = rhs.table[i];
        rhs.table[i] = nullptr;
      }
    }

    // default state for rhs
    rhs.count = 0;
    rhs.capacity = 16;
    rhs.init_table();
  }

  return *this;
}

// destructor
template <typename K, typename V>
HashMap<K, V>::~HashMap()
{
  clear();
}

// Returns the number of key-value pairs in the map
template <typename K, typename V>
int HashMap<K, V>::size() const
{
  return count;
}

// Tests if the map is empty
template <typename K, typename V>
bool HashMap<K, V>::empty() const
{
  if (size() == 0)
  {
    return true;
  }
  return false;
}

// Allows values associated with a key to be updated. Throws
// out_of_range if the given key is not in the collection.
template <typename K, typename V>
V &HashMap<K, V>::operator[](const K &key)
{
  Node *traverse = nullptr;
  int index = -1;

  if (!contains(key))
  {
    throw std::out_of_range("Key is not in the collection");
  }

  index = hash(key);
  traverse = table[index];

  while (traverse != nullptr)
  {
    if (traverse->key == key)
    {
      return traverse->value;
    }
    traverse = traverse->next;
  }
}

// Returns the value for a given key. Throws out_of_range if the
// given key is not in the collection.
template <typename K, typename V>
const V &HashMap<K, V>::operator[](const K &key) const
{
  Node *traverse = nullptr;
  int index = -1;

  if (!contains(key))
  {
    throw std::out_of_range("Key is not in the collection");
  }

  index = hash(key);
  traverse = table[index];

  while (traverse != nullptr)
  {
    if (traverse->key == key)
    {
      return traverse->value;
    }
    traverse = traverse->next;
  }
}

// Extends the collection by adding the given key-value pair.
// Expects key to not exist in map prior to insertion.
template <typename K, typename V>
void HashMap<K, V>::insert(const K &key, const V &value)
{
  double current_ratio = 0.0;
  int index = -1;

  current_ratio = static_cast<double>(count) / capacity;

  if (current_ratio >= load_factor_threshold)
  {
    resize_and_rehash();
  }

  Node *temp = new Node;
  temp->key = key;
  temp->value = value;

  index = hash(key);
  temp->next = table[index];
  table[index] = temp;
  count++;
}

// Shrinks the collection by removing the key-value pair with the
// given key. Does not modify the collection if the collection does
// not contain the key. Throws out_of_range if the given key is not
// in the collection.
template <typename K, typename V>
void HashMap<K, V>::erase(const K &key)
{
  Node *traverse = nullptr;
  Node *remove = nullptr;
  int index = -1;

  if (!contains(key))
  {
    throw std::out_of_range("Key is not in the collection");
  }

  index = hash(key);
  traverse = table[index];

  // remove head
  if (traverse->key == key)
  {
    remove = traverse;
    table[index] = traverse->next;
    delete remove;
    count--;
    return;
  }
  // remove later element
  else
  {
    while (traverse->next != nullptr)
    {
      if (traverse->next->key == key)
      {
        remove = traverse->next;
        traverse->next = remove->next;
        delete remove;
        count--;
        return;
      }
      traverse = traverse->next;
    }
  }
}

// Returns true if the key is in the collection, and false otherwise.
template <typename K, typename V>
bool HashMap<K, V>::contains(const K &key) const
{
  Node *traverse = nullptr;
  int index = -1;

  if (empty())
  {
    return false;
  }

  index = hash(key);
  traverse = table[index];

  while (traverse != nullptr)
  {
    if (traverse->key == key)
    {
      return true;
    }
    traverse = traverse->next;
  }

  return false;
}

// Returns the keys k in the collection such that k1 <= k <= k2
template <typename K, typename V>
ArraySeq<K> HashMap<K, V>::find_keys(const K &k1, const K &k2) const
{
  ArraySeq<K> keys;
  Node *traverse = nullptr;

  for (int i = 0; i < capacity; ++i)
  {
    if (table[i] != nullptr)
    {
      traverse = table[i];
      while (traverse != nullptr)
      {
        if (traverse->key >= k1 && traverse->key <= k2)
        {
          keys.insert(traverse->key, keys.size());
        }
        traverse = traverse->next;
      }
    }
  }
  return keys;
}

// Returns the keys in the collection in ascending sorted order
template <typename K, typename V>
ArraySeq<K> HashMap<K, V>::sorted_keys() const
{
  ArraySeq<K> keys;
  Node *traverse = nullptr;

  for (int i = 0; i < capacity; ++i)
  {
    if (table[i] != nullptr)
    {
      traverse = table[i];
      while (traverse != nullptr)
      {
        keys.insert(traverse->key, keys.size());
        traverse = traverse->next;
      }
    }
  }

  keys.sort();
  return keys;
}

// Gives the key (as an ouptput parameter) immediately after the
// given key according to ascending sort order. Returns true if a
// successor key exists, and false otherwise.
template <typename K, typename V>
bool HashMap<K, V>::next_key(const K &key, K &next_key) const
{
  K successor = key;
  Node *traverse = nullptr;
  bool exists = false;

  for (int i = 0; i < capacity; ++i)
  {
    if (table[i] != nullptr)
    {
      traverse = table[i];

      while (traverse != nullptr)
      {
        if (traverse->key > key)
        {
          exists = true;
          // first time
          if (successor == key)
          {
            successor = traverse->key;
          }
          // not first time AND smaller max
          else if (traverse->key < successor)
          {
            successor = traverse->key;
          }
          // update
          next_key = successor;
        }
        traverse = traverse->next;
      }
    }
  }
  return exists;
}

// Gives the key (as an ouptput parameter) immediately before the
// given key according to ascending sort order. Returns true if a
// predecessor key exists, and false otherwise.
template <typename K, typename V>
bool HashMap<K, V>::prev_key(const K &key, K &next_key) const
{
  K previous = key;
  Node *traverse = nullptr;
  bool exists = false;

  for (int i = 0; i < capacity; ++i)
  {
    if (table[i] != nullptr)
    {
      traverse = table[i];

      while (traverse != nullptr)
      {
        if (traverse->key < key)
        {
          exists = true;
          // first time
          if (previous == key)
          {
            previous = traverse->key;
          }
          // not first time AND larger MIN
          else if (traverse->key > previous)
          {
            previous = traverse->key;
          }
          // update
          next_key = previous;
        }
        traverse = traverse->next;
      }
    }
  }
  return exists;
}

// Removes all key-value pairs from the map. Does not change the
// current capacity of the table.
template <typename K, typename V>
void HashMap<K, V>::clear()
{
  Node *traverse = nullptr;
  Node *next = nullptr;

  for (int i = 0; i < capacity; ++i)
  {
    if (table[i] != nullptr)
    {
      traverse = table[i];
      // clear each node
      while (traverse != nullptr)
      {
        next = traverse->next;
        delete traverse;
        traverse = next;
        count--;
      }
      table[i] = nullptr;
    }
  }

  // clear array
  delete[] table;
}

// statistics functions for the hash table implementation
template <typename K, typename V>
int HashMap<K, V>::min_chain_length() const
{
  int temp = 0, min = 0;
  Node *traverse = nullptr;
  for (int i = 0; i < capacity; ++i)
  {
    if (table[i] != nullptr)
    {
      traverse = table[i];
      // count up amount of elements in each array
      while (traverse != nullptr)
      {
        temp++;
        traverse = traverse->next;
      }
      // first time
      if (min == 0)
      {
        min = temp;
      }
      // every other time
      else if (temp < min)
      {
        min = temp;
      }
      // reset temp counter
      temp = 0;
    }
  }
  return min;
}

template <typename K, typename V>
int HashMap<K, V>::max_chain_length() const
{
  int temp = 0, max = 0;
  Node *traverse = nullptr;
  for (int i = 0; i < capacity; ++i)
  {
    // Only at non-empty buckets
    if (table[i] != nullptr)
    {
      traverse = table[i];
      // count up amount of elements in each array
      while (traverse != nullptr)
      {
        temp++;
        traverse = traverse->next;
      }
      // first time
      if (max == 0)
      {
        max = temp;
      }
      // every other time
      else if (temp > max)
      {
        max = temp;
      }
      // reset temp counter
      temp = 0;
    }
  }
  return max;
}

template <typename K, typename V>
double HashMap<K, V>::avg_chain_length() const
{
  int numArrays = 0;
  Node *traverse = nullptr;
  double avg = 0.0;

  if (size() > 0)
  {
    for (int i = 0; i < capacity; ++i)
    {
      // Only at non-empty buckets
      if (table[i] != nullptr)
      {
        numArrays++;
      }
    }
    avg = static_cast<double>(size()) / numArrays;
  }

  return avg;
}

// the hash function
template <typename K, typename V>
int HashMap<K, V>::hash(const K &key) const
{
  std::hash<K> hash_code;
  int code = hash_code(key);
  int index = code % capacity;

  return index;
}

// resize and rehash the table
template <typename K, typename V>
void HashMap<K, V>::resize_and_rehash()
{
  // old table
  Node **old_table = table;
  int old_cap = capacity;

  // resized table
  capacity = capacity * 2;
  table = new Node *[capacity];
  count = 0;
  init_table();

  Node *traverse = nullptr;
  Node *remove = nullptr;

  // Go through each element
  for (int i = 0; i < old_cap; ++i) // go through current list
  {
    if (old_table[i] != nullptr)
    {
      traverse = old_table[i];
      while (traverse != nullptr)
      {
        insert(traverse->key, traverse->value); // hash will happen inside
        remove = traverse;
        traverse = traverse->next;

        // clear each node;
        delete remove;
      }
      old_table[i] = nullptr;
    }
  }
  // clear old table
  delete[] old_table;
}

// initialize the table to all nullptr
template <typename K, typename V>
void HashMap<K, V>::init_table()
{
  for (int i = 0; i < capacity; ++i)
  {
    table[i] = nullptr;
  }
}

#endif
