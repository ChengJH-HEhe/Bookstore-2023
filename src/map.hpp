#ifndef map_hpp
#define map_hpp

#include<fstream>
#include<cstring>
#include<iostream>
#include"memoryRiver.hpp"
using std::fstream;
using std::pair;
using std::string;
typedef long long ll;

const int mxn = 456;

struct info {
  char str[66];
  ll val = 0;
  info(ll va = 0) { val = va; }
  bool operator<(const info &b) const;
  bool operator==(const info &b) const;
};
struct block_unit {
  int nxt = 0;
  int pos = 0;
  info first;
};
struct __block {
  // adjacent blocksize sums greater than 2*mxn
  int pool[600];
  block_unit message[600];
  int id = 0, head = 0;
  int getid();
  void getback(int);
  // remove id1 (tp = 1) or id1.nxt (tp = 0)
  void remove(int id1, bool tp = 0);
  int addnew(int id1);
};
struct __node {
  info x[2 * mxn + 5] = {};
  int size = 0;
};
struct Map {
  string nameForBlock, nameForMap;
  MemoryRiver<__block> Block;
  MemoryRiver<__node> map;
  
  __block block;
  __node node;

  Map() = default;
  void init(string s1, string s2);
  void read();
  info getinfo(const char *, ll);
  pair<int, int> getpos(info a);
  int find(const char *a);
#define st first
#define nd second
  void insert(pair<int, int> pos, info a);
  void remove(info a) ;
  void printall() ;
#undef st
#undef nd
/*---------More User Friendly---------*/
  
void ins(char*,int);

};

#endif