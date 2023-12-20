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
  Accounts_system::Init();
  int pri = Accounts_system::get_pri();
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
      if(!(stream>>s1)) Books_system::show(s1);
      else if (s1 == "finance") {
        if (pri != 7) {
          invalid();
          continue;
        }
        Log_system::read(stream, 's');
      } else {

        Books_system::show(s1);;
      }
    } else {
      invalid();
    }
  }
}