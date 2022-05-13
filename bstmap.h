//---------------------------------------------------------------------------
// NAME: Joey Macauley
// FILE: bstmap.h
// DATE: CPSC 223 - Spring 2022
// DESC: Map Implementation of a Binary Search Tree
//---------------------------------------------------------------------------

#ifndef BSTMAP_H
#define BSTMAP_H

#include "map.h"
#include "arrayseq.h"

template <typename K, typename V>
class BSTMap : public Map<K, V>
{
public:
  // default constructor
  BSTMap();

  // copy constructor
  BSTMap(const BSTMap &rhs);

  // move constructor
  BSTMap(BSTMap &&rhs);

  // copy assignment
  BSTMap &operator=(const BSTMap &rhs);

  // move assignment
  BSTMap &operator=(BSTMap &&rhs);

  // destructor
  ~BSTMap();

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
  bool prev_key(const K &key, K &prev_key) const;

  // Removes all key-value pairs from the map.
  void clear();

  // Returns the height of the binary search tree
  int height() const;

private:
  // node for linked-list separate chaining
  struct Node
  {
    K key;
    V value;
    Node *left;
    Node *right;
  };

  // number of key-value pairs in map
  int count = 0;

  // array of linked lists
  Node *root = nullptr;

  // clean up the tree and reset count to zero given subtree root
  void clear(Node *st_root);

  // copy assignment helper
  Node *copy(const Node *rhs_st_root) const;

  // erase helper
  Node *erase(const K &key, Node *st_root);

  // find_keys helper
  void find_keys(const K &k1, const K &k2, const Node *st_root,
                 ArraySeq<K> &keys) const;

  // sorted_keys helper
  void sorted_keys(const Node *st_root, ArraySeq<K> &keys) const;

  // height helper
  int height(const Node *st_root) const;
};

template <typename K, typename V>
BSTMap<K, V>::BSTMap()
{
}

// copy constructor
template <typename K, typename V>
BSTMap<K, V>::BSTMap(const BSTMap &rhs)
{
  *this = rhs;
}

// move constructor
template <typename K, typename V>
BSTMap<K, V>::BSTMap(BSTMap &&rhs)
{
  *this = std::move(rhs);
}

// copy assignment
template <typename K, typename V>
BSTMap<K, V> &BSTMap<K, V>::operator=(const BSTMap &rhs)
{
  if (this != &rhs)
  {
    clear();
    root = copy(rhs.root);
    count = rhs.count;
  }
  return *this;
}

// move assignment
template <typename K, typename V>
BSTMap<K, V> &BSTMap<K, V>::operator=(BSTMap &&rhs)
{
  if (this != &rhs)
  {
    clear();
    root = rhs.root;
    count = rhs.count;

    rhs.root = nullptr;
    rhs.count = 0;
  }
  return *this;
}

// destructor
template <typename K, typename V>
BSTMap<K, V>::~BSTMap()
{
  clear();
}

// Returns the number of key-value pairs in the map
template <typename K, typename V>
int BSTMap<K, V>::size() const
{
  return count;
}

// Tests if the map is empty
template <typename K, typename V>
bool BSTMap<K, V>::empty() const
{
  if (root == nullptr)
  {
    return true;
  }
  return false;
}

// Allows values associated with a key to be updated. Throws
// out_of_range if the given key is not in the collection.
template <typename K, typename V>
V &BSTMap<K, V>::operator[](const K &key)
{
  Node *traverse = root;
  while (traverse != nullptr)
  {
    if (key == traverse->key)
    {
      return traverse->value;
    }
    else if (key > traverse->key)
    {
      traverse = traverse->right;
    }
    else
    {
      traverse = traverse->left;
    }
  }
  throw std::out_of_range("Key is not in the collection");
}

// Returns the value for a given key. Throws out_of_range if the
// given key is not in the collection.
template <typename K, typename V>
const V &BSTMap<K, V>::operator[](const K &key) const
{
  Node *traverse = root;
  while (traverse != nullptr)
  {
    if (key == traverse->key)
    {
      return traverse->value;
    }
    else if (key > traverse->key)
    {
      traverse = traverse->right;
    }
    else
    {
      traverse = traverse->left;
    }
  }
  throw std::out_of_range("Key is not in the collection");
}

// Extends the collection by adding the given key-value pair.
// Expects key to not exist in map prior to insertion.
template <typename K, typename V>
void BSTMap<K, V>::insert(const K &key, const V &value)
{
  Node *traverse = root;
  Node *parent = nullptr;
  // Allocate Memory for new leaf
  Node *newLeaf = new Node;
  newLeaf->key = key;
  newLeaf->value = value;
  newLeaf->right = newLeaf->left = nullptr;
  count++;

  // Empty BST
  if (empty())
  {
    root = newLeaf;
  }
  else
  {
    // find location for insertion
    while (traverse != nullptr)
    {
      parent = traverse;
      if (key > traverse->key)
      {
        traverse = traverse->right;
      }
      else
      {
        traverse = traverse->left;
      }
    }
    // Insert to the correct side of parent
    if (key > parent->key)
    {
      parent->right = newLeaf;
    }
    else
    {
      parent->left = newLeaf;
    }
  }
}
// Shrinks the collection by removing the key-value pair with the
// given key. Does not modify the collection if the collection does
// not contain the key. Throws out_of_range if the given key is not
// in the collection.
template <typename K, typename V>
void BSTMap<K, V>::erase(const K &key)
{
  if (empty())
  {
    throw std::out_of_range("Key is not in the collection");
  }
  else if (!contains(key))
  {
    throw std::out_of_range("Key is not in the collection");
  }
  else
  {
    root = erase(key, root);
  }
}

// Returns true if the key is in the collection, and false otherwise.
template <typename K, typename V>
bool BSTMap<K, V>::contains(const K &key) const
{
  Node *traverse = root;

  while (traverse != nullptr)
  {
    if (key == traverse->key)
    {
      return true;
    }
    else if (key > traverse->key)
    {
      traverse = traverse->right;
    }
    else
    {
      traverse = traverse->left;
    }
  }
  return false;
}

// Returns the keys k in the collection such that k1 <= k <= k2
template <typename K, typename V>
ArraySeq<K> BSTMap<K, V>::find_keys(const K &k1, const K &k2) const
{
  ArraySeq<K> keys;
  find_keys(k1, k2, root, keys);
  return keys;
}

// Returns the keys in the collection in ascending sorted order
template <typename K, typename V>
ArraySeq<K> BSTMap<K, V>::sorted_keys() const
{
  ArraySeq<K> keys;
  sorted_keys(root, keys);
  return keys;
}

// Gives the key (as an ouptput parameter) immediately after the
// given key according to ascending sort order. Returns true if a
// successor key exists, and false otherwise.
template <typename K, typename V>
bool BSTMap<K, V>::next_key(const K &key, K &next_key) const
{
  // No keys exist
  if (empty())
  {
    return false;
  }
  // Find next key
  else
  {
    Node *traverse = root;
    Node *parent = nullptr;
    K hold;
    int checkFlag = 0;
    while (traverse != nullptr)
    {
      parent = traverse;
      // found key
      if (key == traverse->key)
      {
        traverse = traverse->right;
        // no right child, but successor still exists
        if (traverse == nullptr && checkFlag == 1)
        {
          next_key = hold;
          return true;
        }
        // no right child
        else if (traverse == nullptr)
        {
          return false;
        }
        // go left left left...
        else
        {
          while (traverse != nullptr)
          {
            parent = traverse;
            traverse = traverse->left;
          }
          next_key = parent->key;
          return true;
        }
      }
      // go right if greater
      else if (key > traverse->key)
      {
        traverse = traverse->right;
      }
      // go left if less, root at the time could be successor
      else
      {
        traverse = traverse->left;
        if (checkFlag == 0)
        {
          hold = parent->key;
        }
        else
        {
          if (hold > parent->key)
          {
            hold = parent->key;
          }
        }

        checkFlag = 1;

        if (traverse == nullptr)
        {
          next_key = hold;
          return true;
        }
      }
    }
    // Key not found, loop exited, successor still exists
    if (checkFlag == 1)
    {
      next_key = hold;
      return true;
    }
    return false;
  }
}

// Gives the key (as an ouptput parameter) immediately before the
// given key according to ascending sort order. Returns true if a
// predecessor key exists, and false otherwise.
template <typename K, typename V>
bool BSTMap<K, V>::prev_key(const K &key, K &prev_key) const
{
  // No keys exist
  if (empty())
  {
    return false;
  }
  // Find prev key
  else
  {
    Node *traverse = root;
    Node *parent = nullptr;
    K hold;
    int checkFlag = 0;

    while (traverse != nullptr)
    {
      parent = traverse;
      // found key
      if (key == traverse->key)
      {
        traverse = traverse->left;
        // prev still exists
        if (traverse == nullptr && checkFlag == 1)
        {
          prev_key = hold;
          return true;
        }
        // no left child
        else if (traverse == nullptr)
        {
          return false;
        }
        // go right right right...
        else
        {
          while (traverse != nullptr)
          {
            parent = traverse;
            traverse = traverse->right;
          }
          prev_key = parent->key;
          return true;
        }
      }
      // go right if greater, root at the time could be predecessor
      else if (key > traverse->key)
      {
        traverse = traverse->right;
        if (checkFlag == 0)
        {
          hold = parent->key;
        }
        else
        {
          if (hold < parent->key)
          {
            hold = parent->key;
          }
        }
        checkFlag = 1;

        if (traverse == nullptr)
        {
          prev_key = hold;
          return true;
        }
      }
      // go left if less
      else
      {
        traverse = traverse->left;
      }
    }
    // Key not found, loop exited, predecessor still exists
    if (checkFlag == 1)
    {
      prev_key = hold;
      return true;
    }
    return false;
  }
}

// Removes all key-value pairs from the map.
template <typename K, typename V>
void BSTMap<K, V>::clear()
{
  clear(root);
}

// Returns the height of the binary search tree
template <typename K, typename V>
int BSTMap<K, V>::height() const
{
  if (empty())
  {
    return 0;
  }
  else
  {
    return height(root);
  }
}

// clean up the tree and reset count to zero given subtree root
template <typename K, typename V>
void BSTMap<K, V>::clear(Node *st_root)
{
  count = 0;

  if (st_root != nullptr)
  {
    if (st_root->left == nullptr and st_root->right == nullptr)
    {
      delete st_root;
      st_root = nullptr;
    }
    else
    {
      clear(st_root->left);
      clear(st_root->right);
      delete st_root;
      st_root = nullptr;
    }
  }

  return;
}

// copy assignment helper
template <typename K, typename V>
typename BSTMap<K, V>::Node *BSTMap<K, V>::copy(const Node *rhs_st_root) const
{
  Node *temp = nullptr;
  if (rhs_st_root != nullptr)
  {
    temp = new Node;
    temp->key = rhs_st_root->key;
    temp->value = rhs_st_root->value;

    temp->left = copy(rhs_st_root->left);
    temp->right = copy(rhs_st_root->right);
  }
  return temp;
}

// erase helper
template <typename K, typename V>
typename BSTMap<K, V>::Node *BSTMap<K, V>::erase(const K &key, Node *st_root)
{
  Node *remove = nullptr;
  Node *successor = nullptr;
  Node *traverse = nullptr;

  if (key < st_root->key)
  {
    st_root->left = erase(key, st_root->left);
  }
  else if (key > st_root->key)
  {
    st_root->right = erase(key, st_root->right);
  }
  else if (key == st_root->key)
  {
    // Case 1: No children
    if (st_root->left == nullptr and st_root->right == nullptr)
    {
      remove = st_root;
      st_root = nullptr;
      delete remove;
      count--;
    }
    // Case 2: One child
    else if (st_root->left == nullptr or st_root->right == nullptr)
    {
      // no left
      if (st_root->left == nullptr)
      {
        remove = st_root;
        st_root = st_root->right;
        delete remove;
        count--;
      }
      // no right
      else
      {
        remove = st_root;
        st_root = st_root->left;
        delete remove;
        count--;
      }
    }
    // Case 3: 2 Children
    else
    {
      // Find inorder successor
      traverse = st_root->right;
      while (traverse != nullptr)
      {
        successor = traverse;
        traverse = traverse->left;
      }

      // copy key and value
      st_root->key = successor->key;
      st_root->value = successor->value;

      // delete and replace successor node
      st_root->right = erase(successor->key, st_root->right);
    }
  }

  return st_root;
}

// find_keys helper
template <typename K, typename V>
void BSTMap<K, V>::find_keys(const K &k1, const K &k2, const Node *st_root, ArraySeq<K> &keys) const
{
  if (st_root == nullptr)
  {
    return;
  }
  if (st_root->key < k1)
  {
    find_keys(k1, k2, st_root->right, keys);
  }
  else if (st_root->key >= k1 and st_root->key <= k2)
  {
    find_keys(k1, k2, st_root->left, keys);
    keys.insert(st_root->key, keys.size());
    find_keys(k1, k2, st_root->right, keys);
  }
  else if (st_root->key > k2)
  {
    find_keys(k1, k2, st_root->left, keys);
  }
}

// sorted_keys helper
template <typename K, typename V>
void BSTMap<K, V>::sorted_keys(const Node *st_root, ArraySeq<K> &keys) const
{
  if (st_root == nullptr)
  {
    return;
  }
  sorted_keys(st_root->left, keys);
  keys.insert(st_root->key, keys.size());
  sorted_keys(st_root->right, keys);
}

// height helper
template <typename K, typename V>
int BSTMap<K, V>::height(const Node *st_root) const
{
  int leftHeight = 0, rightHeight = 0;
  if (st_root == nullptr)
  {
    return 0;
  }
  leftHeight = height(st_root->left);
  rightHeight = height(st_root->right);

  if (leftHeight > rightHeight)
  {
    return ++leftHeight;
  }
  else
  {
    return ++rightHeight;
  }
}

#endif
