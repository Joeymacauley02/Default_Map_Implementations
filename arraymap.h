//---------------------------------------------------------------------------
// NAME: Joey Macauley
// DATE: Spring 2022
// DESC: Implementation of an ArrayMap
//---------------------------------------------------------------------------

#ifndef ARRAYMAP_H
#define ARRAYMAP_H

#include "map.h"
#include "arrayseq.h"

template <typename K, typename V>
class ArrayMap : public Map<K, V>
{
public:
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

  // Returns true if the key is in the collection, and false
  // otherwise.
  bool contains(const K &key) const;

  // Returns the keys k in the collection such that k1 <= k <= k2
  ArraySeq<K> find_keys(const K &k1, const K &k2) const;

  // Returns the keys in the collection in ascending sorted order.
  ArraySeq<K> sorted_keys() const;

  // Gives the key (as an ouptput parameter) immediately after the
  // given key according to ascending sort order. Returns true if a
  // successor key exists, and false otherwise.
  bool next_key(const K &key, K &next_key) const;

  // Gives the key (as an ouptput parameter) immediately before the
  // given key according to ascending sort order. Returns true if a
  // predecessor key exists, and false otherwise.
  bool prev_key(const K &key, K &next_key) const;

  // Removes all key-value pairs from the map.
  void clear();

private:
  // implemented as a resizable array of (key-value) pairs
  ArraySeq<std::pair<K, V>> seq;
};

// TODO: Implement the above functions using the private array
//       sequence object seq. Note that you do not need to define any
//       of the essential operations for this assignment (since they
//       are defined for array seq already)

template <typename K, typename V>
int ArrayMap<K, V>::size() const
{
  return seq.size();
}

// Tests if the map is empty
template <typename K, typename V>
bool ArrayMap<K, V>::empty() const
{
  return seq.empty();
}

// Allows values associated with a key to be updated. Throws
// out_of_range if the given key is not in the collection.
template <typename K, typename V>
V &ArrayMap<K, V>::operator[](const K &key)
{
  for (int i = 0; i < size(); ++i)
  {
    if (seq[i].first == key)
    {
      return seq[i].second;
    }
  }

  throw std::out_of_range("Key is not in the collection");
}

// Returns the value for a given key. Throws out_of_range if the
// given key is not in the collection.
template <typename K, typename V>
const V &ArrayMap<K, V>::operator[](const K &key) const
{
  for (int i = 0; i < size(); ++i)
  {
    if (seq[i].first == key)
    {
      return seq[i].second;
    }
  }

  throw std::out_of_range("Key is not in the collection");
}

// Extends the collection by adding the given key-value pair.
// Expects key to not exist in map prior to insertion.
template <typename K, typename V>
void ArrayMap<K, V>::insert(const K &key, const V &value)
{
  std::pair<K, V> p{key, value};
  seq.insert(p, size());
}

// Shrinks the collection by removing the key-value pair with the
// given key. Does not modify the collection if the collection does
// not contain the key. Throws out_of_range if the given key is not
// in the collection.
template <typename K, typename V>
void ArrayMap<K, V>::erase(const K &key)
{
  if (!contains(key))
  {
    throw std::out_of_range("Key is not in the collection");
  }
  for (int i = 0; i < size(); ++i)
  {
    if (seq[i].first == key)
    {
      seq.erase(i);
      return;
    }
  }
}

// Returns true if the key is in the collection, and false
// otherwise.
template <typename K, typename V>
bool ArrayMap<K, V>::contains(const K &key) const
{
  for (int i = 0; i < size(); ++i)
  {
    if (seq[i].first == key)
    {
      return true;
    }
  }

  return false;
}

// Returns the keys k in the collection such that k1 <= k <= k2
template <typename K, typename V>
ArraySeq<K> ArrayMap<K, V>::find_keys(const K &k1, const K &k2) const
{
  ArraySeq<K> keys;
  for (int i = 0; i < size(); ++i)
  {
    if (seq[i].first >= k1 and seq[i].first <= k2)
    {
      keys.insert(seq[i].first, keys.size());
    }
  }
  return keys;
}

// Returns the keys in the collection in ascending sorted order.
template <typename K, typename V>
ArraySeq<K> ArrayMap<K, V>::sorted_keys() const
{
  ArraySeq<K> sorted_keys;
  for (int i = 0; i < size(); ++i)
  {
    sorted_keys.insert(seq[i].first, sorted_keys.size());
  }
  sorted_keys.sort(); 

  return sorted_keys;
}

// Gives the key (as an ouptput parameter) immediately after the
// given key according to ascending sort order. Returns true if a
// successor key exists, and false otherwise.
template <typename K, typename V>
bool ArrayMap<K, V>::next_key(const K &key, K &next_key) const
{
  K successor = key; 
  bool exists = false; 
  for (int i = 0; i < size(); ++i)
  {
    if (seq[i].first > key)
    {
       exists = true; 
       if (successor == key)
       {
         successor = seq[i].first; 
       }
       else if (seq[i].first < successor)
       {
         successor = seq[i].first;  
       }
       next_key = successor; 
    }
  }
  return exists; 
}

// Gives the key (as an ouptput parameter) immediately before the
// given key according to ascending sort order. Returns true if a
// predecessor key exists, and false otherwise.
template <typename K, typename V>
bool ArrayMap<K, V>::prev_key(const K &key, K &next_key) const
{
  K before = key; 
  bool exists = false; 
  for (int i = 0; i < size(); ++i)
  {
    if (seq[i].first < key)
    {
       exists = true; 
       if (before == key)
       {
         before = seq[i].first; 
       }
       else if (seq[i].first > before)
       {
         before = seq[i].first;  
       }
       next_key = before; 
    }
  }
  return exists;
}

// Removes all key-value pairs from the map.
template <typename K, typename V>
void ArrayMap<K, V>::clear()
{
  seq.clear();
}

#endif
