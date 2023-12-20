#include "books.h"
#include "invalid.h"
#include "map.hpp"
#include <vector>
namespace Books_system {
Map bookMap;

class array{
  std::vector<info> a;
};

namespace print{
  void print(string);
}

void read(std::istringstream &stream, char c1, int pri ) {
  string s[5] = {"@", "@", "@", "@", "@"};
  int sz = 0;
  while (stream >> s[sz++]);
  /*
  4个 char*->int
  ISBN name author keywords
  */

}
void show(string s){
  // h I N A K
  switch(s[1]) {
    case 'h': print::print("all");

  }
}


void Init();
} // namespace Books_system