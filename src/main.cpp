#include <cstring>
#include <iostream>
#include <variant>

// Include the <sstream> header for std::istringstream
#include "Log.h"
#include "accounts.h"
#include "books.h"
using std::cin;

// 实现 读入指令
int main() {
  //freopen("test.in","r",stdin);
  // freopen("test.out","w",stdout);
  std::string s;
  Accounts_system::Init();
  Books_system::Init();
  Log_system::Init();
  while (getline(cin, s)) {
    int pri = Accounts_system::get_pri();
    std::cout<<pri<<std::endl;
    // getpri
    std::istringstream stream(s);
    std::string s1;
    stream >> s1;
    //std::cout << s1 << " \n";
    if (s1 == "quit" || s1 == "exit") {
      Accounts_system::read(stream, s1[0], pri);
      Accounts_system::end();
      Books_system::end();
      Log_system::end();
      return 0;
    } else if (s1 == "su" || s1 == "register" || s1 == "passwd" ||
               s1 == "useradd" || s1 == "delete" || s1 == "logout")
      Accounts_system::read(stream, s1[0], pri);
    else if (s1 == "buy" || s1 == "select" || s1 == "modify" || s1 == "import")
      //std::cout<<s<<std::endl, 
      Books_system::read(stream, s1[0], pri);
      //std::cout<<s<<std::endl;
    else if (s1 == "report") {
      if (pri != 7) {
        invalid();
        continue;
      }
      stream >> s1;
      Log_system::read(stream, s1[0]);
    } else if (s1 == "show") {
      std::string ss = s1;
      if (!(stream >> s1))
        Books_system::show(ss[1], "", pri);
      else if (s1 == "finance") {
        if (pri != 7) {
          invalid();
          continue;
        }
        Log_system::read(stream, 's', 7);
      } else {
        static string str[4] = {"-ISBN=", "-name=\"", "-author=\"",
                                "-keyword=\""};
        switch (s1[1]) {
        case 'I': {
          if (s1.size() <= 6 || s1.substr(0, 6) != str[0])
            invalid();
          else {
            string s = s1.substr(6);
            Books_system::show('I', s, pri);
          }
        } break;
        case 'n': {
          if (s1.size() <= 8 || s1.substr(0, 7) != str[1] || s1.back() != '\"')
            invalid();
          else {
            string s = s1.substr(7); s.pop_back();
            Books_system::show('n', s, pri);
          }
        } break;
        case 'a': {
          if (s1.size() <= 10 || s1.substr(0, 9) != str[2] || s1.back() != '\"')
            invalid();
          else {
            string s = s1.substr(9);s.pop_back();
            Books_system::show('a', s, pri);
          }
        } break;
        case 'k': {
          if (s1.size() <= 11 || s1.substr(0, 10) != str[3] || s1.back() != '\"')
            invalid();
          else {
            string s = s1.substr(10);s.pop_back();
            if (s.find('|') != s.npos)
              invalid();
            else
              Books_system::show('k', s, pri);
          }
        } break;
        default: {
          invalid();
        }
        }
      }
    } else if(s == "log") {
      if(pri != 7) invalid();
      else Log_system::Log();
    }else if(s.size()){
      invalid();
    }
  }
  Accounts_system::end();
  Books_system::end();
  Log_system::end();
}