#ifndef BOOKS_H
#define BOOKS_H

#include<sstream>
#include <vector>
#include "dynamic.h"
#include"map.hpp"
namespace Books_system {
void Init();


namespace books {
  bool convert(const std::string &s);
  long long pd_info(std::string nw, std::string tp);
  struct book {
    char ISBN[21]="", BookName[61]="", Author[61]="";
    char Keywords[61];
    book(const char *a = nullptr){strcpy(ISBN,a);}
    int Quantity = 0;
    double Price = 0.0;
    double TotalCost = 0.0;
  };
  int find_id(const char *s);
  book find_book(int id);
  void add_book(book a, std::vector<std::string> key);
  void delete_book(book a, int id, std::vector<std::string>);
  std::ostream& operator<<(std::ostream& out, const book& nw);

  std::vector<std::string> getkey(string nw);
  bool convert(const std::string &s);
}

void read(std::istringstream&, char c1 = '\0', int pri = 0) ; 
void show(char s, string name);

}
#endif