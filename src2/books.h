#ifndef BOOKS_H
#define BOOKS_H

#include<sstream>
#include <string>
#include <vector>
#include "dynamic.h"
#include"map.hpp"
namespace Books_system {
void Init();
void end();

namespace books {
  bool convert(const std::string &s);
  long long pd_info(std::string nw, std::string tp);
  struct book {
    book() = default;
    char ISBN[21]="\0", BookName[61]="\0", Author[61]="\0";
    char Keywords[61] = "\0";
    int Quantity = 0;
    double Price = 0.0;
    double TotalCost = 0.0;
    book(const char *a){if(a)strcpy(ISBN,a);}
    bool operator< (const book& b) const{
      return strcmp(ISBN, b.ISBN) < 0;
    }
  };
  int find_id(const char *s);
  book find_book(int id);
  string find_book_ISBN(int id);
  int add_book(book a, const std::vector<std::string> &key);
  void delete_book(book a, int id);
  std::ostream& operator<<(std::ostream& out, const book& nw);
  std::vector<std::string> getkey(string nw);
  bool convert(const std::string &s);
}

void read(std::istringstream&, char c1 = '\0', int pri = 0) ; 
void show(char s, string name, int pri = 0);

}
#endif