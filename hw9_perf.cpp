//---------------------------------------------------------------------------
// NAME: S. Bowers
// FILE: hw9_perf.cpp
// DATE: Spring 2022
// DESC: Simple performance test driver for HW-6. To run from the
//       command line use:
//          ./hw9_perf 
//       which will print out the corresponding HW-6 timing data.To
//       save this data to a file, run the command:
//          ./hw9_perf > output.dat
//       This file can then be used by the plotting script to generate
//       the corresponding performance graphs.
//---------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <functional>
#include <vector>
#include <cassert>
#include "util.h"
#include "arrayseq.h"
#include "map.h"
#include "arraymap.h"
#include "binsearchmap.h"
#include "hashmap.h"
#include "bstmap.h"
#include "avlmap.h"

using namespace std;
using namespace std::chrono;

// NOTE: all data is "shuffled"

double timed_insert(Map<int,int>& m, int key);
double timed_erase(Map<int,int>& m, int key);
double timed_contains(const Map<int,int>& m, int key);
double timed_find_range(const Map<int,int>& m, int key1, int key2);
double timed_next_key(const Map<int,int>& m, int key); 
double timed_sorted_keys(const Map<int,int>& m);

// test parameters
const int start = 0;
const int step = 10000; // 5000; // 15000
const int stop = 100000; // 50000; // 150000
const int runs = 3;


int main(int argc, char* argv[])
{
  // configure output
  cout << fixed << showpoint;
  cout << setprecision(2);

  // output data header
  cout << "# All times in milliseconds (msec)" << endl;
  cout << "# Column 1 = input data size" << endl;

  cout << "# Column 2 = binsearch map insert" << endl;
  cout << "# Column 3 = hash map insert" << endl;
  cout << "# Column 4 = bst map insert" << endl;
  cout << "# Column 5 = avl map insert" << endl;
  
  cout << "# Column 6 = binsearch map erase" << endl;
  cout << "# Column 7 = hash map erase" << endl;
  cout << "# Column 8 = bst map erase" << endl;
  cout << "# Column 9 = avl map erase" << endl;
  
  cout << "# Column 10 = binsearch map contains" << endl;
  cout << "# Column 11 = hash map contains" << endl;
  cout << "# Column 12 = bst map contains" << endl;
  cout << "# Column 13 = avl map contains" << endl;
  
  cout << "# Column 14 = binsearch map find range" << endl;
  cout << "# Column 15 = hash map find range" << endl;
  cout << "# Column 16 = bst map find range" << endl;
  cout << "# Column 17 = avl map find range" << endl;
  
  cout << "# Column 18 = binsearch map next key" << endl;
  cout << "# Column 19 = hash map next key" << endl;
  cout << "# Column 20 = bst map next key" << endl;
  cout << "# Column 21 = avl map next key" << endl;
  
  cout << "# Column 22 = binsearch map sorted keys" << endl;
  cout << "# Column 23 = hash map sorted keys" << endl;
  cout << "# Column 24 = bst map sorted keys" << endl;
  cout << "# Column 25 = avl map sorted keys" << endl;
  
  cout << "# Column 26 = bst map height" << endl;
  cout << "# Column 27 = avl map height" << endl;
  cout << "# Column 28 = log base 2 of input size" << endl;  
  
  // generate shuffled data
  ArraySeq<int> keys, vals;
  for (int i = 2; i <= stop*2; i += 2) {
    keys.insert(i, keys.size());
    vals.insert(i, vals.size());
  }
  faro_shuffle(keys, 5);

  // generate the timing data
  for (int n = start; n <= stop; n += step) {
    // load shuffled data
    BinSearchMap<int,int> m1;
    HashMap<int,int> m2;
    BSTMap<int,int> m3;    
    AVLMap<int,int> m4;
    for (int i = 0; i < n; ++i) {
      m1.insert(keys[i], vals[i]);
      m2.insert(keys[i], vals[i]);
      m3.insert(keys[i], vals[i]);
      m4.insert(keys[i], vals[i]);
    }

    int min = 2;
    int med = n;
    int max = n * 2;

    cout << n << " ";
    
    // insert 
    double c2 = timed_insert(m1, med + 1);
    cout << c2 << " " << flush;
    double c3 = timed_insert(m2, med + 1);    
    cout << c3 << " " << flush;
    double c4 = timed_insert(m3, med + 1);
    cout << c4 << " " << flush;
    double c5 = timed_insert(m4, med + 1);
    cout << c5 << " " << flush;

    // erase
    double c6 = timed_erase(m1, med + 1);
    cout << c6 << " " << flush;
    double c7 = timed_erase(m2, med + 1);
    cout << c7 << " " << flush;
    double c8 = timed_erase(m3, med + 1);
    cout << c8 << " " << flush;
    double c9 = timed_erase(m4, med + 1);
    cout << c9 << " " << flush;

    // check sizes
    assert(m1.size() == n);
    assert(m2.size() == n);
    assert(m3.size() == n);
    assert(m4.size() == n);
    
    // contains end
    double c10 = timed_contains(m1, max + 1);
    cout << c10 << " " << flush;
    double c11 = timed_contains(m2, max + 1);
    cout << c11 << " " << flush;
    double c12 = timed_contains(m3, max + 1);
    cout << c12 << " " << flush;
    double c13 = timed_contains(m4, max + 1);
    cout << c13 << " " << flush;

    // key range (1/20th of values)
    double c14 = timed_find_range(m1, med, med + (n/20));
    cout << c14 << " " << flush;
    double c15 = timed_find_range(m2, med, med + (n/20));
    cout << c15 << " " << flush;
    double c16 = timed_find_range(m3, med, med + (n/20));
    cout << c16 << " " << flush;
    double c17 = timed_find_range(m4, med, med + (n/20));
    cout << c17 << " " << flush;

    // find next
    double c18 = timed_next_key(m1, med);
    cout << c18 << " " << flush;
    double c19 = timed_next_key(m2, med);
    cout << c19 << " " << flush;    
    double c20 = timed_next_key(m3, med);
    cout << c20 << " " << flush;    
    double c21 = timed_next_key(m4, med);
    cout << c21 << " " << flush;    
    
    // sort
    double c22 = timed_sorted_keys(m1);
    cout << c22 << " " << flush;
    double c23 = timed_sorted_keys(m2);
    cout << c23 << " " << flush;    
    double c24 = timed_sorted_keys(m3);
    cout << c24 << " " << flush;    
    double c25 = timed_sorted_keys(m4);
    cout << c25 << " " << flush;    
    
    // stats
    int c26 = m3.height();
    cout << c26 << " " << flush;
    int c27 = m4.height();
    cout << c27 << " " << flush;
    int c28 = (n == 0) ? 0 : ceil(log2(n));
    cout << c28 << " " << flush;
    
    cout << endl;
  }
  
}


// assumes keys are multiples of 2 and key to insert is odd
double timed_insert(Map<int,int>& m, int key)
{
  double total = 0;
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    m.insert(key + (r * 2), key + (r * 2));
    auto t1 = high_resolution_clock::now();
    total += duration_cast<microseconds>(t1 - t0).count();
  }
  return (total/1000) / runs;
}

// removes the odd values inserted from timed_insert
double timed_erase(Map<int,int>& m, int key)
{
  double total = 0;
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    m.erase(key + (r * 2));
    auto t1 = high_resolution_clock::now();
    total += duration_cast<microseconds>(t1 - t0).count();
  }
  return (total/1000) / runs;
}

double timed_contains(const Map<int,int>& m, int key) 
{
  double total = 0;
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    m.contains(key);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<microseconds>(t1 - t0).count();
  }
  return (total/1000) / runs;
}

double timed_find_range(const Map<int,int>& m, int key1, int key2)
{
  double total = 0;
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    m.find_keys(key1, key2);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<microseconds>(t1 - t0).count();
  }
  return (total/1000) / runs;
}

double timed_next_key(const Map<int,int>& m, int key)
{
  double total = 0;
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    int next_key;
    m.next_key(key, next_key);
    auto t1 = high_resolution_clock::now();
    total += duration_cast<microseconds>(t1 - t0).count();
  }
  return (total/1000) / runs;
}


double timed_sorted_keys(const Map<int,int>& m)
{
  double total = 0;
  for (int r = 0; r < runs; ++r) {
    auto t0 = high_resolution_clock::now();
    m.sorted_keys();
    auto t1 = high_resolution_clock::now();
    total += duration_cast<microseconds>(t1 - t0).count();
  }
  return (total/1000) / runs;
}




