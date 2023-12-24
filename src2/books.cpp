#include "books.h"
#include "Log.h"
#include "accounts.h"
#include "invalid.h"
#include "map.hpp"
#include <algorithm>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
namespace Books_system {

Map bookMap, keywordMap, nameMap, authorMap;
Dynamic queue("book");
fstream Book; //(file_name Book)
void Init() {
  /*
    book
    book_wait
  */
  Book.open("Book");
  if (!Book.good())
    Book.open("Book", std::ios::out);
  Book.close();
  queue.init();
  bookMap.init("bookAC", "bookNodeAC");
  keywordMap.init("keywordAC", "keywordNodeAC");
  nameMap.init("nameMapAC", "nameMapNodeAC");
  authorMap.init("authorMapAC", "authorMapNodeAC");
}

void end() {
  queue.end();
  bookMap.end();
  keywordMap.end();
  nameMap.end();
  authorMap.end();
}

namespace books {

std::vector<std::string> getkey(string nw) {
  std::vector<std::string> v;
  while (nw.find('|') != nw.npos) {
    int x = nw.find('|');
    if (x == 0) {
      v.clear();
      return v;
    } else {
      v.push_back(nw.substr(0, x));
      if (!pd_info(v.back(), "key")) {
        v.clear();
        return v;
      }
      nw = nw.substr(x + 1);
    }
  }
  if (nw.size()) {
    if (!pd_info(nw, "key") ) {
      v.clear();
      return v;
    } else {
      v.push_back(nw);
    }
  }
  // std::cerr<<nw<<" "<<v.size()<<std::endl;
  return v;
}

long long pd_info(string s, string tp) {
  if (tp == "ISBN") {
    if (s.size() > 20)
      return false;
    for (auto i : s)
      if (iscntrl(i))
        return false;
    return true;
  } else if (tp == "name" || tp == "author" || tp == "key") {
    if (s.size() > 60 || s.empty())
      return false;
    for (auto i : s)
      if (iscntrl(i) || i == '\"')
        return false;
    return true;
  } else if (tp == "Quantity") {
    long long num = 0;
    if(s.size() > 10) return false;
    for (auto i = s.begin(); i != s.end(); ++i)
      if (!isdigit(*i))
        return -1;
      else
        num = num * 10 + (*i - '0'); 
    if( num > INT_MAX)
      num = -1;
    if (s[0] == '0') num = 0;
      return num;
  } else
    return 0;
}
bool convert(const std::string &s) {
  if (s.size() > 13)
    return false;
  int dotpos = -1;
  if(s[0] == '.' || s.back() == '.')return false;
  for (int i = 0; i < s.size(); ++i)
    if (s[i] == '.')
      if (~dotpos)
        return false;
      else
        dotpos = i;
    else if (!isdigit(s[i]))
      return false;
  return true;
}

void modify_key(int id, string past, string nw, char *ISBN) {
  std::vector<std::string> a = getkey(past), b = getkey(nw);
  for (auto ai : a)
    keywordMap.remove(keywordMap.getinfo(ai.c_str(), id));
  for (auto ai : b)
    keywordMap.ins(const_cast<char *>(ai.c_str()), id);
}
bool pd_book_step(int &id, book &nw,const std::string& s1) {
  string str[] = {"-ISBN=", "-name=\"", "-author=\"", "-keyword=\"",
                         "-price="};
  switch (s1[1]) {
  case 'I': {
    // substr第二个参数是长度
    if (s1.size() <= 6 || s1.substr(0, 6) != str[0])
      return false;
    else {
      string s = s1.substr(6);
      if (!pd_info(s, "ISBN") || !strcmp(nw.ISBN, s.c_str()))
        return false;
      if (~find_id(s.c_str()))
        return false;
      return true;
    }
  } break;
  case 'n': {
    if (s1.size() <= 8 || s1.substr(0, 7) != str[1] || s1.back() != '\"')
      return false;
    else {
      string s = s1.substr(7);
      s.pop_back();
      if (!pd_info(s, "name"))
        return false;
      // strcpy(nw.BookName , s.c_str());
      return true;
    }
  } break;
  case 'a': {
    if (s1.size() <= 10 || s1.substr(0, 9) != str[2] || s1.back() != '\"')
      return false;
    else {
      string s = s1.substr(9);
      s.pop_back();
      if (!pd_info(s, "author"))
        return false;
      return true;
    }
  } break;
  case 'k': {
    if (s1.size() <= 11 || s1.substr(0, 10) != str[3] || s1.back() != '\"')
      return false;
    else {
      std::unordered_map<std::string, bool> map;
      string s = s1.substr(10);
      s.pop_back();
      std::vector<std::string> key = getkey(s);
      for (auto i : key)
        if (!pd_info(i, "key") || map.find(i) != map.end())
          return false;
        else
          map[i] = 1;
      if (key.size())
        return true;
      return false;
    }
  } break;
  case 'p': {
    if (s1.size() <= 7 || s1.substr(0, 7) != str[4])
      return false;
    else {
      string s = s1.substr(7);
      return convert(s);
    }
  } break;
  default: {
    return false;
  }
  }
}
void modify_book_step(int &id, book &nw, book &past, const std::string &s1) {
  string str[] = {"-ISBN=", "-name=\"", "-author=\"", "-keyword=\"",
                         "-price="};
  
  switch (s1[1]) {
  case 'I': {
    string s = s1.substr(6);
    delete_book(past, id);
    strcpy(nw.ISBN, s.c_str());
    id = add_book(nw, getkey(nw.Keywords));
    past = nw;
  } break;
  case 'n': {
    string s = s1.substr(7);
    s.pop_back();
    strcpy(nw.BookName, s.c_str());
  } break;
  case 'a': {
    string s = s1.substr(9);
    s.pop_back();
    strcpy(nw.Author, s.c_str());
    // return true;
  } break;
  case 'k': {
    string s = s1.substr(10);
    s.pop_back();
    modify_key(id, nw.Keywords, s, nw.ISBN);
    strcpy(nw.Keywords, s.c_str());
  } break;
  case 'p': {
    string s = s1.substr(7);
    double price = std::stod(s);
    price = std::round(price * 100) / 100;
    nw.Price = price;
  } break;
  default: {
    return;
  }
  }
}
std::ostream &operator<<(std::ostream &out, const book &nw) {
  out << nw.ISBN << '\t' << nw.BookName << '\t' << nw.Author << '\t'
      << nw.Keywords << '\t' << std::fixed << std::setprecision(2) << nw.Price
      << '\t' << nw.Quantity << '\n';
  return out;
}
int find_id(const char *s) {
  // User ID
  return bookMap.find(s);
}
book find_book(int id) {
  // 保证id合法
  book nw;
  Book.open("Book");
  Book.seekg((id - 1) * sizeof(book));
  Book.read(reinterpret_cast<char *>(&nw), sizeof(book));
  Book.close();
  return nw;
}
// string find_book_ISBN(int id) {
//   // 保证id合法
//   char nw[21];
//   Book.open("Book");
//   Book.seekg((id - 1) * sizeof(book));
//   Book.read(reinterpret_cast<char *>(&nw), sizeof(nw));
//   Book.close();
//   return string(nw);
// }

void modify_book(int id, book &past, book &nw, int tp = 0) {
  if (tp == 0) {
    Book.open("Book");
    if (!Book.good()) {
      Book.open("Book", std::ios::out);
    }
    Book.seekp((id - 1) * sizeof(book));
    Book.write(reinterpret_cast<char *>(&nw), sizeof(book));
    Book.close();
  } else if (tp == 1) {
    authorMap.remove(authorMap.getinfo(past.Author, id));
    authorMap.ins(nw.Author, id);
  } else if (tp == 2) {
    nameMap.remove(nameMap.getinfo(past.BookName, id));
    nameMap.ins(nw.BookName, id);
  }
}

int add_book(book a, const std::vector<std::string> &key) {
  int id = queue.getid();
  bookMap.ins(a.ISBN, id);
  Book.open("Book");
  if (!Book.good()) {
    Book.open("Book", std::ios::out);
  }
  Book.seekp((id - 1) * sizeof(book));
  Book.write(reinterpret_cast<char *>(&a), sizeof(book));
  Book.close();
  // std::cout<<a<<" "<<"bookinfo"<< std::endl;
  if (a.BookName[0] != '\0')
    nameMap.ins(a.BookName, id);
  if (a.Author[0] != '\0')
    authorMap.ins(a.Author, id);
  for (auto i : key)
    keywordMap.ins(const_cast<char *>(i.c_str()), id);
  return id;
}
void delete_book(book a, int id) {
  queue.getback(id);
  std::vector<std::string> key = getkey(a.Keywords);
  bookMap.remove(bookMap.getinfo(a.ISBN, id));
  if (a.BookName[0] != '\0')
    nameMap.remove(nameMap.getinfo(a.BookName, id));
  if (a.Author[0] != '\0')
    authorMap.remove(authorMap.getinfo(a.Author, id));
  for (auto i : key)
    keywordMap.remove(keywordMap.getinfo(const_cast<char *>(i.c_str()), id));
}

} // namespace books

void read(std::istringstream &stream, char c1, int pri) {
  string s[10];
  int sz = 0;
  while (sz<=5 && stream >> s[sz++])
    ;
  --sz;
  switch (c1) {
  case 'b': {
    if (pri < 1)
      return invalid();
    if (sz != 2 || !books::pd_info(s[0], "ISBN")) {
      return invalid();
    } else {
      long long num = 0;
      if(s[1].size() > 10) return invalid();
      for (auto i = s[1].begin(); i != s[1].end(); ++i)
        if (!isdigit(*i))
          return invalid();
        else
          num = num * 10 + (*i - '0'); 
      if(s[1][0] == '0') num = 0;
      if(num > INT_MAX) return invalid();

      int id = bookMap.find(s[0].c_str());
      if (id == -1)
        return invalid();
      books::book nw = books::find_book(id);
      if (nw.Quantity < num || num == 0)
        return invalid();

      nw.Quantity -= num;
      modify_book(id, nw, nw);
      Log_system::add(num * nw.Price);
      printf("%.2lf\n", num * nw.Price);
    }
  } break;
  case 's': {
    if (pri < 3 || sz != 1 || !books::pd_info(s[0], "ISBN"))
      return invalid();
    int id = bookMap.find(s[0].c_str());
    if (id == -1)
      id = books::add_book(books::book(s[0].c_str()),
                           std::vector<std::string>());
    Accounts_system::stack::select(id);
  } break;
  case 'm': {
    if (pri < 3 || !sz)
      return invalid();
    int id = Accounts_system::stack::back().book;
    if (!id)
      return invalid();
    std::unordered_map<char, bool> pd;
    for (int i = 0; i < sz; ++i)
      if (s[i].size() <= 6 || !isalpha(s[i][1]) || pd[s[i][1]])
        return invalid();
      else
        pd[s[i][1]] = 1;
    books::book nw = books::find_book(id), nw2 = nw;
    for (int i = 0; i < sz; ++i)
      if (!pd_book_step(id, nw, s[i]))
        return invalid();
    for (int i = 0; i < sz; ++i) {
      modify_book_step(id, nw, nw2, s[i]);
    }
    if (!strcmp(nw2.ISBN, nw.ISBN))
      modify_book(id, nw2, nw, 0);
    if (strcmp(nw2.Author, nw.Author))
      modify_book(id, nw2, nw, 1);
    if (strcmp(nw2.BookName, nw.BookName))
      modify_book(id, nw2, nw, 2);

  } break;
  case 'i': {
    // import [Quantity] [TotalCost]
    if (sz != 2 || pri < 3) {
      return invalid();
    } else {
      long long num = books::pd_info(s[0], "Quantity");
      if (num <= 0 || !books::convert(s[1]))
        return invalid();
      double TotalCost = std::stod(s[1]);
      TotalCost = round(TotalCost * 100) / 100;
      int id = Accounts_system::stack::back().book;
      if (!id || TotalCost < 1e-3)
        return invalid();
      books::book nw = books::find_book(id);
      nw.Quantity += num;
      modify_book(id, nw, nw);
      Log_system::add(-TotalCost);
    }
  } break;
  }
}
void show(char s, string name, int pri) {
  static std::vector<int> st;
  st.clear();
  // h I N A K
  // 遍历map输出
  if (pri < 1)
    return invalid();
  switch (s) {
  case 'h': {
    std::vector<int> st;
    bookMap.multifind(st, nullptr);
    if (st.empty())
      std::cout << '\n';
    else {
      for (auto i : st)
        std::cout << books::find_book(i);
    }
  } break;
  case 'I': {
    if(!books::pd_info(name,"ISBN"))
      return invalid();
    int id = bookMap.find(name.c_str());
    if (~id)
      std::cout << books::find_book(id);
    else
      std::cout << '\n';
  } break;
  case 'n': {
    if(!books::pd_info(name,"name"))
      return invalid();
    nameMap.multifind(st, name.c_str());
  } break;
  case 'a': {
    if(!books::pd_info(name,"author"))
      return invalid();
    authorMap.multifind(st, name.c_str());
  } break;
  case 'k': {
    if(!books::pd_info(name,"key"))
      return invalid();
    keywordMap.multifind(st, name.c_str());
  } break;
  }
  if (s == 'n' || s == 'a' || s == 'k') {
    if (st.empty())
      std::cout << '\n';
    else {
      std::vector<std::pair<books::book, int>> v_ISBN;
      v_ISBN.clear();
      for (auto i : st)
        v_ISBN.push_back(std::move(std::make_pair((books::find_book(i)), i)));
      std::sort(v_ISBN.begin(), v_ISBN.end());
      for (auto i : v_ISBN) {
        std::cout << i.first;
      }
    }
  }
}

} // namespace Books_system