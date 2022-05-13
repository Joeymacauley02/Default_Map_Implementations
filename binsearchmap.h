//---------------------------------------------------------------------------
// NAME: Joey Macauley
// FILE: binsearchmap.h
// DATE: CPSC 223 - Spring 2022
// DESC: Implementation of BinSearchMap (uses binary search)
//---------------------------------------------------------------------------

#ifndef BINSEARCHMAP_H
#define BINSEARCHMAP_H

#include "map.h"
#include "arrayseq.h"

template <typename K, typename V>
class BinSearchMap : public Map<K, V>
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

  // Extends the collection by adding the given key-value
  // pair. Assumes the key being added is not present in the
  // collection. Insert does not check if the key is present.
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
  bool prev_key(const K &key, K &prev_key) const;

  // Removes all key-value pairs from the map.
  void clear();

private:
  // If the key is in the collection, bin_search returns true and
  // provides the key's index within the array sequence (via the index
  // output parameter). If the key is not in the collection,
  // bin_search returns false and provides the last index checked by
  // the binary search algorithm.
  bool bin_search(const K &key, int &index) const;

  // implemented as a resizable array of (key-value) pairs
  ArraySeq<std::pair<K, V>> seq;
};

template <typename K, typename V>
int BinSearchMap<K, V>::size() const
{
  return seq.size();
}

// Tests if the map is empty
template <typename K, typename V>
bool BinSearchMap<K, V>::empty() const
{
  return seq.empty();
}

// Allows values associated with a key to be updated. Throws
// out_of_range if the given key is not in the collection.
template <typename K, typename V>
V &BinSearchMap<K, V>::operator[](const K &key)
{
  int index = -1;
  bool exists = false;
  exists = bin_search(key, index);
  if (!exists)
  {
    throw std::out_of_range("Key is not in the collection");
  }
  else
  {
    return seq[index].second;
  }
}

// Returns the value for a given key. Throws out_of_range if the
// given key is not in the collection.
template <typename K, typename V>
const V &BinSearchMap<K, V>::operator[](const K &key) const
{
  int index = -1;
  bool exists = false;
  exists = bin_search(key, index);
  if (!exists)
  {
    throw std::out_of_range("Key is not in the collection");
  }
  else
  {
    return seq[index].second;
  }
}

// Extends the collection by adding the given key-value
// pair. Assumes the key being added is not present in the
// collection. Insert does not check if the key is present.
template <typename K, typename V>
void BinSearchMap<K, V>::insert(const K &key, const V &value)
{
  int index = -1;
  std::pair<K, V> p{key, value};
  bool exists = false;
  exists = bin_search(key, index);

  if (index < 0)
  {
    seq.insert(p, size());
  }
  else if (!exists)
  {
    if (key < seq[index].first)
    {
      seq.insert(p, index);
    }
    else
    {
      index = index + 1;
      seq.insert(p, index);
    }
  }
}

// Shrinks the collection by removing the key-value pair with the
// given key. Does not modify the collection if the collection does
// not contain the key. Throws out_of_range if the given key is not
// in the collection.
template <typename K, typename V>
void BinSearchMap<K, V>::erase(const K &key)
{

  int index = -1;
  bool exists = false;
  exists = bin_search(key, index);

  if (!exists)
  {
    throw std::out_of_range("Key is not in the collection");
  }
  else
  {
    seq.erase(index);
    return;
  }
}

// Returns true if the key is in the collection, and false
// otherwise.
template <typename K, typename V>
bool BinSearchMap<K, V>::contains(const K &key) const
{
  int index = -1;
  return bin_search(key, index);
}

// Returns the keys k in the collection such that k1 <= k <= k2
template <typename K, typename V>
ArraySeq<K> BinSearchMap<K, V>::find_keys(const K &k1, const K &k2) const
{
  ArraySeq<K> keys;
  int start = -1;
  bool exists = bin_search(k1, start);
  if (start < 0)
  {
    return keys;
  }
  else if (k1 > seq[start].first)
  {
    start = start + 1;
  }

  for (int i = start; i < size(); i++)
  {
    if (seq[i].first > k2)
    {
      return keys;
    }
    else
    {
      keys.insert(seq[i].first, keys.size());
    }
  }
  return keys;
}

// Returns the keys in the collection in ascending sorted order.
template <typename K, typename V>
ArraySeq<K> BinSearchMap<K, V>::sorted_keys() const
{
  ArraySeq<K> sorted_keys;
  for (int i = 0; i < size(); ++i)
  {
    sorted_keys.insert(seq[i].first, sorted_keys.size());
  }
  return sorted_keys;
}

// Gives the key (as an ouptput parameter) immediately after the
// given key according to ascending sort order. Returns true if a
// successor key exists, and false otherwise.
template <typename K, typename V>
bool BinSearchMap<K, V>::next_key(const K &key, K &next_key) const
{
  int index = -1;
  bool exists = false;
  exists = bin_search(key, index);

  if (!exists)
  {
    if (index < 0)
    {
      return false;
    }
    else if (key < seq[index].first)
    {
      index = index - 1;
    }
  }

  index = index + 1;

  if (index > size() - 1)
  {
    return false;
  }
  else
  {
    next_key = seq[index].first;
    return true;
  }
}

// Gives the key (as an ouptput parameter) immediately before the
// given key according to ascending sort order. Returns true if a
// predecessor key exists, and false otherwise.
template <typename K, typename V>
bool BinSearchMap<K, V>::prev_key(const K &key, K &prev_key) const
{
  int index = -1;
  bool exists = false;
  exists = bin_search(key, index);

  if (!exists)
  {
    if (index < 0)
    {
      return false;
    }
    else if (key > seq[index].first)
    {
      index = index + 1;
    }
  }

  index = index - 1;

  if (index < 0)
  {
    return false;
  }
  else
  {
    prev_key = seq[index].first;
    return true;
  }
}

// Removes all key-value pairs from the map.
template <typename K, typename V>
void BinSearchMap<K, V>::clear()
{
  seq.clear();
}

// If the key is in the collection, bin_search returns true and
// provides the key's index within the array sequence (via the index
// output parameter). If the key is not in the collection,
// bin_search returns false and provides the last index checked by
// the binary search algorithm.
template <typename K, typename V>
bool BinSearchMap<K, V>::bin_search(const K &key, int &index) const
{
  int start = 0;
  int end = size() - 1;
  int mid = 0;

  if (empty())
  {
    return false;
  }

  while (start <= end)
  {
    mid = (start + end) / 2;

    if (key == seq[mid].first)
    {
      index = mid;
      return true;
    }
    else if (key < seq[mid].first)
    {
      end = mid - 1;
    }
    else
    {
      start = mid + 1;
    }
  }

  index = mid;
  return false;
}

#endif
