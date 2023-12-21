#include <cstring>
#include <iostream>
#include <variant>

// Include the <sstream> header for std::istringstream
#include "accounts.h"
#include "books.h"
#include "log.h"
using std::cin;

char s[100000];
// 实现 读入指令
int main() {
  std::string s;
  Accounts_system::stack::Init();
  int pri = Accounts_system::get_pri();

  Books_system::Init();

  while (getline(cin, s)) {
    // getpri
    std::istringstream stream(s);
    std::string s1;
    stream >> s1;
    if (s1 == "su" || s1 == "register" || s1 == "passwd" || s1 == "useradd" ||
        s1 == "delete" || s1 == "logout" || s1 == "quit" || s1 == "exit")
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
      if(!(stream>>s1)) Books_system::show(s[1], "");
      else if (s1 == "finance") {
        if (pri != 7) {
          invalid();
          continue;
        }
        Log_system::read(stream, 's');
      } else {
        static string str[4] ={"-ISBN=", "-name=\"","-author=\"","-keyword=\""}; 
        switch(s1[1]) {
          case 'I': {
            if(s1.size() <=  6 || s1.substr(0,5) != str[0]) invalid();
            else {
              string s = s1.substr(6);
              Books_system::show('I', s);
            }
          } break;
          case 'n': {
            if(s1.size() <= 8 || s1.substr(0,6) != str[1] || s1.back() != '\"') invalid();
            else {
              string s = s1.substr(7, s1.size() - 1);
              Books_system::show('n', s);
            }
          } break;
          case 'a': {
            if(s1.size() <= 10 || s1.substr(0,8) != str[2] || s1.back() != '\"') invalid();
            else {
              string s = s1.substr(9, s1.size() - 1);
              Books_system::show('a', s);
            }
          } break;
          case 'k': {
            if(s1.size() <= 11 || s1.substr(0,9) != str[3] || s1.back() != '\"') invalid();
            else {
              string s = s1.substr(10, s1.size() - 1);
              if(s.find('|') != s.npos) invalid(); 
              else Books_system::show('a', s);
            }
          } break;
          default: {
            invalid();
          }
        }
      }
    } else {
      invalid();
    }
  }
}