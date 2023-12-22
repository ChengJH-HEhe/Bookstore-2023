#ifndef BOOKS_H
#define BOOKS_H

#include<sstream>
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
    char ISBN[21]="", BookName[61]="", Author[61]="";
    char Keywords[61] = "";
    int Quantity = 0;
    double Price = 0.0;
    double TotalCost = 0.0;
    book(const char *a){if(a)strcpy(ISBN,a);}
  };
  int find_id(const char *s);
  book find_book(int id);
  int add_book(book a, std::vector<std::string> key);
  void delete_book(book a, int id);
  std::ostream& operator<<(std::ostream& out, const book& nw);
  std::vector<std::string> getkey(string nw);
  bool convert(const std::string &s);
}

void read(std::istringstream&, char c1 = '\0', int pri = 0) ; 
void show(char s, string name, int pri = 0);

}
#endif