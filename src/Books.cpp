#include "books.h"
#include "Log.h"
#include "accounts.h"
#include "invalid.h"
#include "map.hpp"
#include <cassert>
#include <climits>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <stdexcept>
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
  if(!Book.good()) Book.open("Book",std::ios::out);
  Book.close();
  bookMap.init("bookAC", "bookNodeAC");
  keywordMap.init("keywordAC", "keywordNodeAC");
  nameMap.init("nameMapAC", "nameMapNodeAC");
  authorMap.init("authorMapAC", "authorMapNodeAC");
  
}

void end() {
  queue.end();
}

namespace books {

std::vector<std::string> getkey(string nw) {
  std::unordered_map<std::string,bool> mp;
  std::vector<std::string> v;
  while(nw.find('|') != nw.npos) {
    int x = nw.find('|');
    if(x == 0) {
      v.clear();
      return v;
    } else {
      v.push_back(nw.substr(0, x));
      if(mp.find(v.back()) != mp.end() || !pd_info(v.back(), "key")){
        v.clear();
        return v;
      }
      nw = nw.substr(x+1);
    }
  }
  if(nw.size() && (!pd_info(nw, "key") || mp.find(nw)!= mp.end())) {
    v.clear();
    return v;
  } else {
    v.push_back(nw);
  }
  return v;
}

long long pd_info(string s, string tp) {
  if(tp == "ISBN") {
    if(s.size()>20) return false;
    for(auto i : s) if(iscntrl(i)) return false;
    return true;
  } else if(tp == "name" || tp == "author" || tp == "key") {
    if(s.size()>60 || s.empty()) return false;
    for(auto i : s) if(iscntrl(i)|| i == '\"')return false;
    return true;
  } else if(tp == "Quantity") {
    long long num = 0;
    for (auto i = s.begin(); i != s.end(); ++i)
        if (!isdigit(*i) || num > INT_MAX)
          return 0; 
        else
          num = num * 10 + (*i - '0');
    return num;
  } else return 0;
}
bool convert(const std::string &s) {
  try {
    std::stod(s);
    return true;
  } catch (const std::invalid_argument &) {
    return false;
  }
}

bool pd_book_step(int& id, book& nw, std::string s1) {
  static string str[] = {"-ISBN=", "-name=\"", "-author=\"", "-keyword=\"", "-price="};
  switch (s1[1]) {
  case 'I': {
    // 第二个参数是长度
    if (s1.size() <= 6 || s1.substr(0, 6) != str[0])
      return false;
    else {
      string s = s1.substr(6);
      if(!strcmp(nw.ISBN, s.c_str()) || !pd_info(s,"ISBN"))
        return false;
      if(~find_id(s.c_str())) return false;
      // delete_book(id, nw);
      // strcpy(nw.ISBN, s.c_str());
      return true;
    }
  } break;
  case 'n': {
    if (s1.size() <= 8 || s1.substr(0, 7) != str[1] || s1.back() != '\"')
      return false;
    else {
       string s = s1.substr(7);s.pop_back();
       if(!pd_info(s,"name")) return false;
      // strcpy(nw.BookName , s.c_str());
      return true;
    }
  } break;
  case 'a': {
    if (s1.size() <= 10 || s1.substr(0, 9) != str[2] || s1.back() != '\"')
      return false;
    else {
       string s = s1.substr(9);s.pop_back();
      if(!pd_info(s, "author"))return false;
      return true;
    }
  } break;
  case 'k': {
    if (s1.size() <= 11 || s1.substr(0, 10) != str[3] || s1.back() != '\"')
      return false;
    else {
      string s = s1.substr(10);s.pop_back();
      std::vector<std::string> key = getkey(s);
      for(auto i : key)
        if(!pd_info(i, "key")) return false;
      if(key.size()) return true;
      return false;
    }
  } break;
  case 'p': {
    if (s1.size() <= 7 || s1.substr(0, 7) != str[4] )
      return false;
    else {
      string s = s1.substr(7);
      if(convert(s)) return true;
      return false;
    }
  }break;
  default: {
    return false;
  }
  }
}
void modify_book_step(int& id, book& nw, book& past, std::string s1) {
  static string str[] = {"-ISBN=", "-name=\"", "-author=\"", "-keyword=\"", "-price="};
  switch (s1[1]) {
  case 'I': {
      string s = s1.substr(6);
      delete_book(past, id, getkey(past.Keywords));
      strcpy(nw.ISBN, s.c_str());
      add_book(nw, getkey(nw.Keywords));
      past = nw;
  } break;
  case 'n': {
      string s = s1.substr(7); s.pop_back();
      strcpy(nw.BookName , s.c_str());
  } break;
  case 'a': {
      string s = s1.substr(9);s.pop_back();
      strcpy(nw.Author, s.c_str());
      // return true;
  } break;
  case 'k': {
      string s = s1.substr(10);s.pop_back();
      strcpy(nw.Keywords , s.c_str());
      //TODO
  } break;
  case 'p': {
      string s = s1.substr(7);
      double price = std::stod(s);
      //std::cout<<s<<" "<<std::fixed<<" "<< price<<" "<<std::endl;
      nw.Price = price;
  }break;
  default: {
    return;
  }
  }
}
std::ostream &operator<<(std::ostream &out, const book &nw) {
  out << nw.ISBN << '\t' << nw.BookName << '\t' << nw.Author << '\t'
      << nw.Keywords << '\t' << std::fixed << std::setprecision(2) << nw.Price << '\t'
      << nw.Quantity << '\n';
  return out;
}
int find_id(const char *s) {
  // User ID
  int id = bookMap.find(s);
  if (~id) {
    return id;
  }
  return -1;
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
void modify_book(int id,book past, book nw) {
  Book.open("Book");
  if (!Book.good()) {
    Book.open("Book", std::ios::out);
  }
  Book.seekp((id - 1) * sizeof(book));
  Book.write(reinterpret_cast<char *>(&nw), sizeof(book));
  Book.close();
  if(strcmp(past.Author, nw.Author)){
    authorMap.remove(authorMap.getinfo(strcat(past.Author,past.ISBN), id));
    authorMap.ins(strcat(nw.Author,nw.ISBN), id);
  }
  if(strcmp(past.BookName, nw.BookName)){
    nameMap.remove(nameMap.getinfo(strcat(past.BookName,past.ISBN), id));
    nameMap.ins(strcat(nw.BookName,nw.ISBN), id);
  }
}

int add_book(book a, std::vector<std::string> key) {
  int id = queue.getid();
  bookMap.ins(a.ISBN, id);
  modify_book(id, a, a);
  if(a.BookName[0] != '\0') nameMap.ins(strcat(a.BookName, a.ISBN),id);
  if(a.Author[0] != '\0') authorMap.ins(strcat(a.Author, a.ISBN), id);
  for (auto i : key)
    keywordMap.ins(std::strcat(const_cast<char *>(i.c_str()),a.ISBN), id);
  return id;
}
void delete_book(book a, int id, std::vector<std::string> key) {
  queue.getback(id);
  bookMap.remove(bookMap.getinfo(a.ISBN, id));
  if(strcmp(a.BookName,"\0")) nameMap.remove(nameMap.getinfo(strcat(a.BookName,a.ISBN), id));
  if(strcmp(a.Author,"\0"))authorMap.remove(authorMap.getinfo(strcat(a.Author,a.ISBN), id));
  for (auto i : key)
    keywordMap.remove(keywordMap.getinfo(strcat(const_cast<char*>(i.c_str()), a.ISBN), id));
}

} // namespace books


void modify_key(int id, std::vector<std::string> a, std::vector<std::string> b, char* ISBN) {
  for(auto ai:a) 
    keywordMap.remove(keywordMap.getinfo(strcat(const_cast<char*>(ai.c_str()), ISBN), id));
  for(auto ai:b) 
    keywordMap.ins(strcat(const_cast<char*>(ai.c_str()), ISBN), id);
}

void read(std::istringstream &stream, char c1, int pri) {
  string s[5] = {"@", "@", "@", "@", "@"};
  int sz = 0;
  while (stream >> s[sz++]); 
  --sz;
  /*
  4个 char*->int
  ISBN name author keywords
  buy
  s1 == "buy" 1 buy [ISBN] [Quantity]
  s1 == "select" 3 select [ISBN]
  s1 == "modify" 3 modify (-ISBN=[ISBN] | -name="[BookName]" |
  -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+ s1 == "import" 3
  import [Quantity] [TotalCost]
  */
  switch (c1) {
  case 'b': {
    if (!pri)
      return invalid();
    if (sz != 2 || !books::pd_info(s[0], "ISBN")) {
      return invalid();
    } else {
      long long num = 0;
      for (auto i = s[1].begin(); i != s[1].end(); ++i)
        if (!isdigit(*i) || num > INT_MAX)
          return invalid();
        else
          num = num * 10 + (*i - '0');
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
    //////////////////////////////std::cout<< pri << std::endl; 忘传pri
    if (pri < 3 || sz != 1 || !books::pd_info(s[0],"ISBN"))
      return invalid();
    int id = bookMap.find(s[0].c_str());
    if (id == -1)
      id = books::add_book(books::book(s[0].c_str()), std::vector<std::string>());
    Accounts_system::stack::select(id);
  } break;
  case 'm': {
    int id = Accounts_system::stack::back().book;
    //std::cout<<id<<" "<<std::endl;
    if(!id) return invalid();
    bool pd[200];
    memset(pd,0,sizeof(pd));
    for(int i = 0; i < sz; ++i)
      if(s[i].size() <= 6 || !isalpha(s[i][1]) || pd[s[i][1]-'A'])
        return invalid();
      else pd[s[i][1]-'A'] = 1;
    books::book nw , nw2;
    nw = books::find_book(id), nw2 = nw;
    for(int i = 0; i < sz; ++i)
      if(!pd_book_step(id, nw, s[i]))
        return invalid();
    //assert(0);
    for(int i = 0; i < sz; ++i)
      modify_book_step(id, nw, nw2, s[i]);
    modify_key(id,books::getkey(nw2.Keywords), books::getkey(nw.Keywords), nw.ISBN);
    modify_book(id, nw2, nw);
  } break;
  case 'i': {
    // import [Quantity] [TotalCost]
    if (sz != 2) {
      return invalid();
    } else {
      long long num = books::pd_info(s[0],"Quantity");
      if(num < 0 || !books::convert(s[1])) return invalid();
      double TotalCost = std::stod(s[1]);
      int id = Accounts_system::stack::back().book;
      if (!id || TotalCost < 1e-6)
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
  // h I N A K
  // 遍历map输出
  if(pri < 1) return invalid();
  switch (s) {
  case 'h': {
    std::vector<int> st = bookMap.multifind(nullptr);
    std::cerr<<st.size();
    if (st.empty())
      std::cout << '\n';
    else {
      for (auto i : st)
        std::cout << books::find_book(i);
    }
  } break;
  case 'I': {
    int id = bookMap.find(name.c_str());
    if (~id)
      std::cout << books::find_book(id);
    else
      std::cout << '\n';
  } break;
  case 'n': {
    std::vector<int> st = nameMap.multifind(name.c_str());
    if (st.empty())
      std::cout << '\n';
    else {
      for (auto i : st)
        std::cout << books::find_book(i);
    }
  } break;
  case 'a': {
    std::vector<int> st = authorMap.multifind(name.c_str());
    if (st.empty())
      std::cout << '\n';
    else {
      for (auto i : st)
        std::cout << books::find_book(i);
    }
  } break;
  case 'k': {
    std::vector<int> st = keywordMap.multifind(name.c_str());
    if (st.empty())
      std::cout << '\n';
    else {
      for (auto i : st)
        std::cout << books::find_book(i);
    }
  } break;
  }
}

} // namespace Books_system