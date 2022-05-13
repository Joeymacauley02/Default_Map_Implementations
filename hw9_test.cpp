//---------------------------------------------------------------------------
// FILE: hw9_test.cpp
// NAME: S. Bowers
// DATE: Spring 2022
// DESC: Basic unit tests for HW-9
//---------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "arrayseq.h"
#include "avlmap.h"

using namespace std;

//----------------------------------------------------------------------
// Basic Tests for the AVLMap implementation of Map
//----------------------------------------------------------------------

TEST(BasicAVLMapTests, EmptyCheck)
{
  AVLMap<char, int> m;
  ASSERT_EQ(true, m.empty());
  ASSERT_EQ(0, m.size());
}

TEST(BasicAVLMapTests, InsertCheck)
{
  AVLMap<char, int> m;
  m.insert('a', 10);
  m.insert('b', 20);
  m.insert('c', 30);
  m.insert('d', 40);
  ASSERT_EQ(false, m.empty());
  ASSERT_EQ(4, m.size());
}

TEST(BasicAVLMapTests, RValueAccessCheck)
{
  AVLMap<char, int> m;
  m.insert('a', 10);
  m.insert('b', 20);
  m.insert('c', 30);
  m.insert('d', 40);
  ASSERT_EQ(4, m.size());
  ASSERT_EQ(10, m['a']);
  ASSERT_EQ(20, m['b']);
  ASSERT_EQ(30, m['c']);
  ASSERT_EQ(40, m['d']);
}

TEST(BasicAVLMapTests, LValueAccessCheck)
{
  AVLMap<char, int> m;
  m.insert('a', 10);
  m.insert('b', 20);
  m.insert('c', 30);
  m.insert('d', 40);
  m['a'] = 40;
  m['b'] = 30;
  m['c'] = 20;
  m['d'] = 10;
  ASSERT_EQ(40, m['a']);
  ASSERT_EQ(30, m['b']);
  ASSERT_EQ(20, m['c']);
  ASSERT_EQ(10, m['d']);
}

TEST(BasicAVLMapTests, SimpleContainsCheck)
{
  AVLMap<char, int> m;
  m.insert('a', 10);
  m.insert('b', 20);
  m.insert('c', 30);
  m.insert('d', 40);
  ASSERT_EQ(true, m.contains('a'));
  ASSERT_EQ(true, m.contains('b'));
  ASSERT_EQ(true, m.contains('c'));
  ASSERT_EQ(true, m.contains('d'));
  ASSERT_EQ(false, m.contains('e'));
}

TEST(BasicAVLMapTests, MoreInvolvedContainsCheck)
{
  AVLMap<char, int> m;
  for (int i = 'z'; i >= 'a'; --i)
    m.insert(i, (int)i);
  for (int i = 'a'; i <= 'z'; ++i)
    ASSERT_EQ(true, m.contains(i));
}

TEST(BasicAVLMapTests, EraseCheck)
{
  AVLMap<char, int> m;
  m.insert('h', 45);
  m.insert('d', 25);
  m.insert('b', 15);
  m.insert('a', 10);
  m.insert('c', 20);
  m.insert('f', 35);
  m.insert('e', 30);
  m.insert('g', 40);
  m.insert('l', 65);
  m.insert('j', 55);
  m.insert('i', 50);
  m.insert('k', 60);
  m.insert('n', 75);
  m.insert('m', 70);
  m.insert('o', 80);
  ASSERT_EQ(15, m.size());
  m.erase('h');
  ASSERT_EQ(14, m.size());
  ASSERT_EQ(false, m.contains('h'));
  m.erase('i');
  ASSERT_EQ(13, m.size());
  ASSERT_EQ(false, m.contains('i'));
  m.erase('k');
  ASSERT_EQ(12, m.size());
  ASSERT_EQ(false, m.contains('k'));
  m.erase('j');
  ASSERT_EQ(11, m.size());
  ASSERT_EQ(false, m.contains('j'));
  m.erase('c');
  ASSERT_EQ(10, m.size());
  ASSERT_EQ(false, m.contains('c'));
  m.erase('b');
  ASSERT_EQ(9, m.size());
  ASSERT_EQ(false, m.contains('b'));
  m.erase('a');
  ASSERT_EQ(8, m.size());
  ASSERT_EQ(false, m.contains('a'));
  m.erase('d');
  ASSERT_EQ(7, m.size());
  ASSERT_EQ(false, m.contains('d'));
  m.erase('l');
  ASSERT_EQ(6, m.size());
  ASSERT_EQ(false, m.contains('l'));
  m.erase('m');
  ASSERT_EQ(5, m.size());
  ASSERT_EQ(false, m.contains('m'));
  m.erase('f');
  ASSERT_EQ(4, m.size());
  ASSERT_EQ(false, m.contains('f'));
  m.erase('n');
  ASSERT_EQ(3, m.size());
  ASSERT_EQ(false, m.contains('h'));
  m.erase('o');
  ASSERT_EQ(2, m.size());
  ASSERT_EQ(false, m.contains('o'));
  m.erase('e');
  ASSERT_EQ(1, m.size());
  ASSERT_EQ(false, m.contains('e'));
  m.erase('g');
  ASSERT_EQ(0, m.size());
  ASSERT_EQ(false, m.contains('g'));
}

TEST(BasicAVLMapTests, KeyRangeCheck)
{
  AVLMap<char, int> m;
  m.insert('e', 40);
  m.insert('c', 20);
  m.insert('b', 10);
  m.insert('d', 30);
  m.insert('g', 60);
  m.insert('f', 50);
  m.insert('h', 70);
  ASSERT_EQ(7, m.size());
  ArraySeq<char> k;
  k = m.find_keys('c', 'g');
  ASSERT_EQ(5, k.size());
  for (int i = 0; i < 5; ++i)
    ASSERT_EQ('c' + i, k[i]);
  k = m.find_keys('d', 'f');
  ASSERT_EQ(3, k.size());
  for (int i = 0; i < 3; ++i)
    ASSERT_EQ('d' + i, k[i]);
  k = m.find_keys('a', 'd');
  ASSERT_EQ(3, k.size());
  for (int i = 0; i < 3; ++i)
    ASSERT_EQ('b' + i, k[i]);
  k = m.find_keys('f', 'i');
  ASSERT_EQ(3, k.size());
  for (int i = 0; i < 3; ++i)
    ASSERT_EQ('f' + i, k[i]);
  k = m.find_keys('a', 'i');
  ASSERT_EQ(7, k.size());
  for (int i = 0; i < 7; ++i)
    ASSERT_EQ('b' + i, k[i]);
}

TEST(BasicAVLMapTests, SortedKeyCheck)
{
  AVLMap<char, int> m;
  m.insert('e', 50);
  m.insert('a', 10);
  m.insert('c', 30);
  m.insert('b', 20);
  m.insert('d', 40);
  ArraySeq<char> k;
  k = m.sorted_keys();
  ASSERT_EQ(5, k.size());
  ASSERT_EQ('a', k[0]);
  ASSERT_EQ('b', k[1]);
  ASSERT_EQ('c', k[2]);
  ASSERT_EQ('d', k[3]);
  ASSERT_EQ('e', k[4]);
}

TEST(BasicAVLMapTests, InvalidKeyCheck)
{
  AVLMap<char, int> m;
  int x = 10;
  EXPECT_THROW(m['a'] = x, std::out_of_range);
  EXPECT_THROW(x = m['a'], std::out_of_range);
  EXPECT_THROW(m.erase('a'), std::out_of_range);
  m.insert('a', 10);
  m.insert('c', 30);
  EXPECT_THROW(m['b'] = x, std::out_of_range);
  EXPECT_THROW(x = m['b'], std::out_of_range);
  EXPECT_THROW(m.erase('b'), std::out_of_range);
}

TEST(BasicAVLMapTests, NextKeyCheck)
{
  AVLMap<char, int> m;
  m.insert('e', 50);
  m.insert('a', 10);
  m.insert('c', 30);
  m.insert('d', 40);
  m.insert('h', 80);
  m.insert('g', 70);
  char next_key = 0;
  ASSERT_EQ(true, m.next_key('a', next_key));
  ASSERT_EQ('c', next_key);
  next_key = 0;
  ASSERT_EQ(true, m.next_key('b', next_key));
  ASSERT_EQ('c', next_key);
  next_key = 0;
  ASSERT_EQ(true, m.next_key('c', next_key));
  ASSERT_EQ('d', next_key);
  next_key = 0;
  ASSERT_EQ(true, m.next_key('d', next_key));
  ASSERT_EQ('e', next_key);
  next_key = 0;
  ASSERT_EQ(true, m.next_key('e', next_key));
  ASSERT_EQ('g', next_key);
  next_key = 0;
  ASSERT_EQ(true, m.next_key('f', next_key));
  ASSERT_EQ('g', next_key);
  next_key = 0;
  ASSERT_EQ(true, m.next_key('g', next_key));
  ASSERT_EQ('h', next_key);
  ASSERT_EQ(false, m.next_key('h', next_key));
}

TEST(BasicAVLMapTests, PrevKeyCheck)
{
  AVLMap<char, int> m;
  m.insert('e', 50);
  m.insert('a', 10);
  m.insert('c', 30);
  m.insert('d', 40);
  m.insert('h', 80);
  m.insert('g', 70);
  char prev_key = 0;
  ASSERT_EQ(false, m.prev_key('a', prev_key));
  prev_key = 0;
  ASSERT_EQ(true, m.prev_key('b', prev_key));
  ASSERT_EQ('a', prev_key);
  prev_key = 0;
  ASSERT_EQ(true, m.prev_key('c', prev_key));
  ASSERT_EQ('a', prev_key);
  prev_key = 0;
  ASSERT_EQ(true, m.prev_key('d', prev_key));
  ASSERT_EQ('c', prev_key);
  prev_key = 0;
  ASSERT_EQ(true, m.prev_key('e', prev_key));
  ASSERT_EQ('d', prev_key);
  prev_key = 0;
  ASSERT_EQ(true, m.prev_key('f', prev_key));
  ASSERT_EQ('e', prev_key);
  prev_key = 0;
  ASSERT_EQ(true, m.prev_key('g', prev_key));
  ASSERT_EQ('e', prev_key);
  prev_key = 0;
  ASSERT_EQ(true, m.prev_key('h', prev_key));
  ASSERT_EQ('g', prev_key);
}

TEST(BasicAVLMapTests, DestructorCheck)
{
  AVLMap<char, int> *m = new AVLMap<char, int>;
  EXPECT_NO_THROW(delete m);
  m = new AVLMap<char, int>;
  m->insert('a', 1);
  EXPECT_NO_THROW(delete m);
  m = new AVLMap<char, int>;
  for (char c = 'a'; c <= 'z'; ++c)
    m->insert(c, (int)(c - 'a'));
  EXPECT_NO_THROW(delete m);
  m = new AVLMap<char, int>;
  m->insert('e', 40);
  m->insert('c', 20);
  m->insert('b', 10);
  m->insert('d', 30);
  m->insert('g', 60);
  m->insert('f', 50);
  m->insert('h', 70);
  EXPECT_NO_THROW(delete m);
}

TEST(BasicAVLMapTests, CopyConstructorCheck)
{
  // copy empty seq
  AVLMap<char, int> m1;
  AVLMap<char, int> m2(m1);
  ASSERT_EQ(m1.size(), m2.size());
  // copy one-element seq
  m1.insert('a', 1);
  ASSERT_NE(m1.size(), m2.size());
  m2.insert('b', 2);
  m2.insert('c', 3);
  m2.insert('d', 4);
  AVLMap<char, int> m3(m2);
  ASSERT_EQ(m2.size(), m3.size());
  // remove item and check two seqs are different
  m3.erase('c');
  ASSERT_NE(m2.size(), m3.size());
  ASSERT_EQ(true, m2.contains('c'));
  ASSERT_EQ(false, m3.contains('c'));
}

TEST(BasicAVLMapTests, CopyAssignmentCheck)
{
  AVLMap<char, int> m1;
  AVLMap<char, int> m2;
  // both empty
  m2 = m1;
  ASSERT_EQ(0, m1.size());
  ASSERT_EQ(0, m2.size());
  // add to m1 (shouldn't change m2)
  m1.insert('a', 0);
  m1.insert('b', 1);
  ASSERT_EQ(2, m1.size());
  ASSERT_EQ(0, m2.size());
  ASSERT_EQ(true, m1.contains('a') and m1.contains('b'));
  // assign m 1 to itself (shouldn't change anything)
  m1 = m1;
  ASSERT_EQ(2, m1.size());
  ASSERT_EQ(true, m1.contains('a') and m1.contains('b'));
  // add items to m2 and assign m2 to m1
  m2.insert('c', 0);
  m2.insert('d', 1);
  m2.insert('e', 2);
  m1 = m2;
  ASSERT_EQ(3, m2.size());
  ASSERT_EQ(3, m1.size());
  ASSERT_EQ(true, m1.contains('c') and m2.contains('c'));
  ASSERT_EQ(true, m1.contains('d') and m2.contains('d'));
  ASSERT_EQ(true, m1.contains('e') and m2.contains('e'));
  // add to m1 (shouldn't change m2)
  m1.insert('f', 3);
  ASSERT_EQ(4, m1.size());
  ASSERT_EQ(3, m2.size());
  ASSERT_EQ(true, not m2.contains('f') and m1.contains('f'));
}

TEST(BasicAVLMapTests, MoveConstructorCheck)
{
  // move empty map
  AVLMap<char, int> m1;
  AVLMap<char, int> m2(std::move(m1));
  ASSERT_EQ(0, m1.size());
  ASSERT_EQ(0, m2.size());
  // move one-element ma[
  AVLMap<char, int> m3;
  m3.insert('a', 1);
  ASSERT_EQ(1, m3.size());
  ASSERT_EQ(true, m3.contains('a'));
  AVLMap<char, int> m4(std::move(m3));
  ASSERT_EQ(0, m3.size());
  ASSERT_EQ(1, m4.size());
  ASSERT_EQ(true, m4.contains('a'));
  // add item to new m (to ensure correct move)
  m4.insert('b', 2);
  ASSERT_EQ(0, m3.size());
  ASSERT_EQ(2, m4.size());
  ASSERT_EQ(true, m4.contains('a') and m4.contains('b'));
}

TEST(BasicAVLMapTests, MoveAssignmentCheck)
{
  // create a map
  AVLMap<char, int> m1;
  m1.insert('a', 1);
  m1.insert('b', 2);
  m1.insert('c', 3);
  ASSERT_EQ(3, m1.size());
  // and another
  AVLMap<char, int> m2;
  m2.insert('d', 4);
  ASSERT_EQ(1, m2.size());
  // move first to second
  m2 = std::move(m1);
  ASSERT_EQ(0, m1.size());
  ASSERT_EQ(3, m2.size());
  ASSERT_EQ(true, m2.contains('a') && m2.contains('b') && m2.contains('c'));
  m1.insert('e', 5);
  m1.insert('f', 6);
  ASSERT_EQ(2, m1.size());
  ASSERT_EQ(true, m1.contains('e') && m1.contains('f'));
  ASSERT_EQ(false, m2.contains('e') || m2.contains('f'));
  // create an empty map
  AVLMap<char, int> m3;
  ASSERT_EQ(0, m3.size());
  // move second to third
  m3 = std::move(m2);
  ASSERT_EQ(3, m3.size());
  ASSERT_EQ(0, m2.size());
  ASSERT_EQ(2, m1.size());
  ASSERT_EQ(true, m3.contains('a') && m3.contains('b') && m3.contains('c'));
}

TEST(BasicAVLMapTests, HeightEmptyCheck)
{
  AVLMap<int, int> m;
  ASSERT_EQ(0, m.size());
  ASSERT_EQ(0, m.height());
}

TEST(BasicAVLMapTests, SimpleHeightCheck)
{
  // create a map
  AVLMap<char, int> m;
  m.insert('e', 40);
  ASSERT_EQ(1, m.height());
  m.insert('b', 10);
  ASSERT_EQ(2, m.height());
  m.insert('g', 60);
  ASSERT_EQ(2, m.height());
  m.insert('d', 30);
  ASSERT_EQ(3, m.height());
  m.insert('h', 70);
  ASSERT_EQ(3, m.height());
  m.insert('c', 20);
  ASSERT_EQ(3, m.height());
  m.insert('f', 50);
  ASSERT_EQ(3, m.height());
}

TEST(BasicAVLMapTests, InsertRebalanceCheck)
{
  // left rotation
  AVLMap<string, int> c1;
  c1.insert("a", 10);
  c1.insert("b", 20);
  c1.insert("c", 30);
  ASSERT_EQ(2, c1.height());
  // right rotation
  AVLMap<string, int> c2;
  c2.insert("c", 30);
  c2.insert("b", 20);
  c2.insert("a", 10);
  ASSERT_EQ(2, c2.height());
  // left-right rotation
  AVLMap<string, int> c3;
  c3.insert("c", 30);
  c3.insert("a", 10);
  c3.insert("b", 20);
  ASSERT_EQ(2, c3.height());
  // right-left rotation
  AVLMap<string, int> c4;
  c4.insert("a", 10);
  c4.insert("c", 30);
  c4.insert("b", 20);
  ASSERT_EQ(2, c4.height());
  // right rotation
  AVLMap<string, int> c5;
  c5.insert("e", 50);
  c5.insert("f", 60);
  c5.insert("c", 30);
  c5.insert("d", 40);
  c5.insert("b", 20);
  c5.insert("a", 10);
  ASSERT_EQ(3, c5.height());
  // right-left rotation
  AVLMap<string, int> c6;
  c6.insert("e", 50);
  c6.insert("f", 60);
  c6.insert("b", 20);
  c6.insert("d", 40);
  c6.insert("a", 10);
  c6.insert("c", 30);
  ASSERT_EQ(3, c6.height());
  // right rotation
  AVLMap<string, int> c7;
  c7.insert("b", 20);
  c7.insert("a", 10);
  c7.insert("d", 40);
  c7.insert("c", 30);
  c7.insert("e", 50);
  c7.insert("f", 60);
  ASSERT_EQ(3, c7.height());
  // left-right rotation
  AVLMap<string, int> c8;
  c8.insert("b", 20);
  c8.insert("a", 10);
  c8.insert("e", 50);
  c8.insert("d", 40);
  c8.insert("f", 60);
  c8.insert("c", 30);
  ASSERT_EQ(3, c8.height());
}

TEST(BasicAVLMapTests, EraseRebalanceCheck)
{
  // no children, rebalance
  AVLMap<string, int> c1;
  c1.insert("c", 30);
  c1.insert("b", 20);
  c1.insert("d", 40);
  c1.insert("a", 10);
  ASSERT_EQ(3, c1.height());
  c1.erase("c");
  ASSERT_EQ(2, c1.height());
  // one child, rebalance
  AVLMap<string, int> c2;
  c2.insert("e", 50);
  c2.insert("c", 30);
  c2.insert("f", 60);
  c2.insert("a", 10);
  c2.insert("d", 40);
  c2.insert("b", 20);
  c2.insert("g", 70);
  ASSERT_EQ(4, c2.height());
  c2.erase("f");
  ASSERT_EQ(3, c2.height());
  c2.erase("a");
  ASSERT_EQ(3, c2.height());
  c2.erase("b");
  ASSERT_EQ(3, c2.height());
  // two children, rebalance
  AVLMap<string, int> c3;
  c3.insert("e", 50);
  c3.insert("b", 20);
  c3.insert("g", 70);
  c3.insert("a", 10);
  c3.insert("d", 40);
  c3.insert("f", 60);
  c3.insert("c", 30);
  ASSERT_EQ(4, c3.height());
  c3.erase("e");
  ASSERT_EQ(3, c3.height());
}

//----------------------------------------------------------------------
// Main
//----------------------------------------------------------------------

int main(int argc, char *argv[])
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
