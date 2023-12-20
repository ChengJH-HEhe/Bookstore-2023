#ifndef BOOKS_H
#define BOOKS_H

#include<sstream>
#include"map.hpp"
namespace Books_system {
void Init();



void read(const std::istringstream&, char c1 = '\0', int pri = 0) ; 
void show(string s);


}
#endif