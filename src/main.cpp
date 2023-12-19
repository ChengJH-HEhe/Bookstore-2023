#include <cstring>
#include <iostream>

// Include the <sstream> header for std::istringstream
#include "Accounts.h"
#include "Books.h"
#include "Log.h"
using std::cin;

char s[100000];
// 实现 读入指令
int main() {
  std::string s;
  int pri = Accounts_system::current_pri();
  Accounts_system::Init();
  Books_system::Init();
  Log_system::Init();
  while (getline(cin, s)) {
    // getpri
    std::istringstream stream(s);
    std::string s1;
    stream >> s1;
    if (s1 == "su" || s1 == "register" || s1 == "passwd" || s1 == "useradd" ||
        s1 == "delete" || s1 == "logout")
      Accounts_system::read(stream, s1[0], pri);
    else if (s1 == "buy" || s1 == "select" || s1 == "modify" || s1 == "import")
      Books_system::read(stream, s1[0]);
    else if (s1 == "report") {
      if (pri != 7) {
        invalid();
        continue;
      }
      stream >> s1;
      Log_system::read(stream, s1[0]);
    } else if (s1 == "show") {
      stream >> s1;
      if (s1 == "finance") {
        if (pri != 7) {
          invalid();
          continue;
        }
        Log_system::read(stream, 's');
      } else
        Books_system::read(stream, 'S');
    } else {
      invalid();
    }
  }
}