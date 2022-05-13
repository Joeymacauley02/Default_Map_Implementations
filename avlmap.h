//---------------------------------------------------------------------------
// NAME: Joey Macauley
// FILE: avlmap.h
// DATE: CPSC 223 - Spring 2022
// DESC: Implementation of AVL Map using ArraySeq.h and Map.h
//---------------------------------------------------------------------------

#ifndef AVLMAP_H
#define AVLMAP_H

#include "map.h"
#include "arrayseq.h"

template <typename K, typename V>
class AVLMap : public Map<K, V>
{
public:
  // default constructor
  AVLMap();

  // copy constructor
  AVLMap(const AVLMap &rhs);

  // move constructor
  AVLMap(AVLMap &&rhs);

  // copy assignment
  AVLMap &operator=(const AVLMap &rhs);

  // move assignment
  AVLMap &operator=(AVLMap &&rhs);

  // destructor
  ~AVLMap();

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

  // helper to print the tree for debugging
  void print() const;

private:
  // node for linked-list separate chaining
  struct Node
  {
    K key;
    V value;
    int height;
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

  // insert helper
  Node *insert(const K &key, const V &value, Node *st_root);

  // erase helper
  Node *erase(const K &key, Node *st_root);

  // find_keys helper
  void find_keys(const K &k1, const K &k2, const Node *st_root, ArraySeq<K> &keys) const;

  // sorted_keys helper
  void sorted_keys(const Node *st_root, ArraySeq<K> &keys) const;

  // rotations
  Node *rotate_right(Node *k2);
  Node *rotate_left(Node *k2);

  // rebalance
  Node *rebalance(Node *st_root);

  // print helper
  void print(std::string indent, const Node *st_root) const;
};

template <typename K, typename V>
void AVLMap<K, V>::print() const
{
  print(std::string(""), root);
}

template <typename K, typename V>
void AVLMap<K, V>::print(std::string indent, const Node *st_root) const
{
  if (!st_root)
    return;
  std::cout << st_root->key << " (" << st_root->height << ")" << std::endl;
  if (st_root->left)
  {
    std::cout << indent << " lft: ";
    print(indent + " ", st_root->left);
  }
  if (st_root->right)
  {
    std::cout << indent << " rgt: ";
    print(indent + " ", st_root->right);
  }
}

template <typename K, typename V>
AVLMap<K, V>::AVLMap()
{
}

// TODO: Finish the remaining functions below. Many of the functions
// for this assignment can be taken from HW8. Note that for helper
// functions that return Node*, you must include the template
// information for the type, e.g.:
//   template<typename K, typename V>
//   typename AVLMap<K,V>::Node* AVLMap<K,V>::rotate_right(Node* k2)

// copy constructor
template <typename K, typename V>
AVLMap<K, V>::AVLMap(const AVLMap &rhs)
{
  *this = rhs;
}

// move constructor
template <typename K, typename V>
AVLMap<K, V>::AVLMap(AVLMap &&rhs)
{
  *this = std::move(rhs);
}

// copy assignment
template <typename K, typename V>
AVLMap<K, V> &AVLMap<K, V>::operator=(const AVLMap &rhs)
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
AVLMap<K, V> &AVLMap<K, V>::operator=(AVLMap &&rhs)
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
AVLMap<K, V>::~AVLMap()
{
  clear();
}

// Returns the number of key-value pairs in the map
template <typename K, typename V>
int AVLMap<K, V>::size() const
{
  return count;
}

// Tests if the map is empty
template <typename K, typename V>
bool AVLMap<K, V>::empty() const
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
V &AVLMap<K, V>::operator[](const K &key)
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
const V &AVLMap<K, V>::operator[](const K &key) const
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
void AVLMap<K, V>::insert(const K &key, const V &value)
{
  root = insert(key, value, root);
}

// Shrinks the collection by removing the key-value pair with the
// given key. Does not modify the collection if the collection does
// not contain the key. Throws out_of_range if the given key is not
// in the collection.
template <typename K, typename V>
void AVLMap<K, V>::erase(const K &key)
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
bool AVLMap<K, V>::contains(const K &key) const
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
ArraySeq<K> AVLMap<K, V>::find_keys(const K &k1, const K &k2) const
{
  ArraySeq<K> keys;
  find_keys(k1, k2, root, keys);
  return keys;
}

// Returns the keys in the collection in ascending sorted order
template <typename K, typename V>
ArraySeq<K> AVLMap<K, V>::sorted_keys() const
{
  ArraySeq<K> keys;
  sorted_keys(root, keys);
  return keys;
}

// Gives the key (as an ouptput parameter) immediately after the
// given key according to ascending sort order. Returns true if a
// successor key exists, and false otherwise.
template <typename K, typename V>
bool AVLMap<K, V>::next_key(const K &key, K &next_key) const
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
bool AVLMap<K, V>::prev_key(const K &key, K &prev_key) const
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
void AVLMap<K, V>::clear()
{
  clear(root);
}

// Returns the height of the binary search tree
template <typename K, typename V>
int AVLMap<K, V>::height() const
{
  if (empty())
  {
    return 0;
  }
  return root->height;
}

// clean up the tree and reset count to zero given subtree root
template <typename K, typename V>
void AVLMap<K, V>::clear(Node *st_root)
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
typename AVLMap<K, V>::Node *AVLMap<K, V>::copy(const Node *rhs_st_root) const
{
  Node *temp = nullptr;
  if (rhs_st_root != nullptr)
  {
    temp = new Node;
    temp->key = rhs_st_root->key;
    temp->value = rhs_st_root->value;
    temp->height = rhs_st_root->height;

    temp->left = copy(rhs_st_root->left);
    temp->right = copy(rhs_st_root->right);
  }
  return temp;
}

// insert helper
template <typename K, typename V>
typename AVLMap<K, V>::Node *AVLMap<K, V>::insert(const K &key, const V &value, Node *st_root)
{
  if (st_root == nullptr)
  {
    Node *newLeaf = new Node;
    newLeaf->key = key;
    newLeaf->value = value;
    newLeaf->height = 1;
    newLeaf->right = nullptr;
    newLeaf->left = nullptr;
    st_root = newLeaf;
    count++;
  }
  else
  {
    if (key < st_root->key)
    {
      st_root->left = insert(key, value, st_root->left);
    }
    else
    {
      st_root->right = insert(key, value, st_root->right);
    }
  }
  // Adjust height correctly
  if (st_root->left && !st_root->right)
  {
    st_root->height = st_root->left->height + 1;
  }
  else if (!st_root->left && st_root->right)
  {
    st_root->height = st_root->right->height + 1;
  }
  else if (st_root->left && st_root->right)
  {
    if (st_root->left->height >= st_root->right->height)
    {
      st_root->height = st_root->left->height + 1;
    }
    else
    {
      st_root->height = st_root->right->height + 1;
    }
  }
  return rebalance(st_root);
}

// erase helper
template <typename K, typename V>
typename AVLMap<K, V>::Node *AVLMap<K, V>::erase(const K &key, Node *st_root)
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

  // Adjust height correctly
  if (st_root)
  {
    if (st_root->left && !st_root->right)
    {
      st_root->height = st_root->left->height + 1;
    }
    else if (!st_root->left && st_root->right)
    {
      st_root->height = st_root->right->height + 1;
    }
    else if (st_root->left && st_root->right)
    {
      if (st_root->left->height >= st_root->right->height)
      {
        st_root->height = st_root->left->height + 1;
      }
      else
      {
        st_root->height = st_root->right->height + 1;
      }
    }
    else // leaf node
    {
      st_root->height = 1;
    }
  }
  return rebalance(st_root);
}

// find_keys helper
template <typename K, typename V>
void AVLMap<K, V>::find_keys(const K &k1, const K &k2, const Node *st_root, ArraySeq<K> &keys) const
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
void AVLMap<K, V>::sorted_keys(const Node *st_root, ArraySeq<K> &keys) const
{
  if (st_root == nullptr)
  {
    return;
  }
  sorted_keys(st_root->left, keys);
  keys.insert(st_root->key, keys.size());
  sorted_keys(st_root->right, keys);
}

// rotations
template <typename K, typename V>
typename AVLMap<K, V>::Node *AVLMap<K, V>::rotate_right(Node *k2)
{
  Node *k1 = k2->left;
  k2->left = k1->right;
  if (k1->right && k1->left && k1->left->height == k1->right->height)
  {
    k1->height += 1;
  }
  k1->right = k2;
  return k1;
}

template <typename K, typename V>
typename AVLMap<K, V>::Node *AVLMap<K, V>::rotate_left(Node *k2)
{
  Node *k1 = k2->right;
  k2->right = k1->left;
  if (k1->right && k1->left && k1->left->height == k1->right->height)
  {
    k1->height += 1;
  }
  k1->left = k2;
  return k1;
}

// rebalance
template <typename K, typename V>
typename AVLMap<K, V>::Node *AVLMap<K, V>::rebalance(Node *st_root)
{
  if (st_root == nullptr)
  {
    return st_root;
  }

  Node *l_ptr = st_root->left;
  Node *r_ptr = st_root->right;

  int bf = 0;
  // left, no right
  if (l_ptr && !r_ptr && l_ptr->height > 1)
  {
    // right, no left
    if (l_ptr->right && !l_ptr->left && l_ptr->right->height > 1)
    {
      l_ptr = rotate_left(l_ptr);
      l_ptr->left->height = l_ptr->height - 1;
    }
    // left and right
    else if (l_ptr->right && l_ptr->left)
    {
      bf = l_ptr->left->height - l_ptr->right->height;
      if (bf < -1)
      {
        l_ptr = rotate_left(l_ptr);
        l_ptr->left->height = l_ptr->height - 1;
      }
    }
    // left, no right (left-left)
    st_root = rotate_right(st_root);
    st_root->right->height = st_root->height - 1;
  }
  // right, no left
  else if (r_ptr && !l_ptr && r_ptr->height > 1)
  {
    // left, no right
    if (r_ptr->left && !r_ptr->right && r_ptr->left->height > 1)
    {
      r_ptr = rotate_right(r_ptr);
      r_ptr->right->height = r_ptr->height - 1;
    }
    // left and right
    else if (r_ptr->left && r_ptr->right)
    {
      bf = r_ptr->left->height - r_ptr->right->height;
      if (bf > 1)
      {
        r_ptr = rotate_right(r_ptr);
        r_ptr->right->height = r_ptr->height - 1;
      }
    }
    // right, no left (right-right)
    st_root = rotate_left(st_root);
    st_root->left->height = st_root->height - 1;
  }
  // left and right
  else if (l_ptr != nullptr && r_ptr != nullptr)
  {
    bf = l_ptr->height - r_ptr->height;
    // left heavy
    if (bf > 1)
    {
      // inserted right (double rotation)
      if (l_ptr->right && l_ptr->left)
      {
        bf = l_ptr->left->height - l_ptr->right->height;
        if (bf < -1)
        {
          l_ptr = rotate_left(l_ptr);
          l_ptr->left->height = l_ptr->height - 1;
        }
      }
      st_root = rotate_right(st_root);
      st_root->right->height = st_root->height - 1;
    }
    // right heavy
    else if (bf < -1)
    {
      // inserted left (double rotation)
      if (r_ptr->left && r_ptr->right)
      {
        bf = r_ptr->left->height - r_ptr->right->height;
        if (bf > 1)
        {
          r_ptr = rotate_right(r_ptr);
          r_ptr->right->height = r_ptr->height - 1;
        }
      }
      st_root = rotate_left(st_root);
      st_root->left->height = st_root->height - 1;
    }
  }

  return st_root;
}
#endif
