#include <cstring>
#include <iostream>
#include <ostream>
#include <regex>
#include <sstream>
#include <variant>

// Include the <sstream> header for std::istringstream
#include "Log.h"
#include "accounts.h"
#include "books.h"
using std::cin;

// 实现 读入指令

bool onlySpaces(const std::string &str) {
  for (char c : str) {
    if (std::isspace(c) && c != ' ') {
      return false;
    }
  }
  return true;
}

void show(std::istringstream &stream,std::string s1, int pri) {
  std::string s[10];
  int sz = 0;
  while(sz<=4 && stream>>s[sz++]);
  --sz;
  if(!(sz <= 2)) return invalid();
  if (sz == 0) {
    Books_system::show(s1[1], "", pri);
  } else if (s[0] == "finance") {
    if (pri != 7) {
      invalid();    
      return;
    }
    Log_system::read((sz>1?s[1]:""), 's', 7);
  } else {
    if(!pri || sz != 1) return invalid();
    static string str[4] = {"-ISBN=", "-name=\"", "-author=\"", "-keyword=\""};
    s1 = s[0];
    switch (s[0][1]) {
    case 'I': {
      if (s1.size() <= 6 || s1.substr(0, 6) != str[0]) {
        invalid();
        return;
      } else {
        string s = s1.substr(6);
        Books_system::show('I', s, pri);
      }
    } break;
    case 'n': {
      if (s1.size() <= 8 || s1.substr(0, 7) != str[1] || s1.back() != '\"') {
        invalid();
      } else {
        string s = s1.substr(7);
        s.pop_back();
        Books_system::show('n', s, pri);
      }
    } break;
    case 'a': {
      if (s1.size() <= 10 || s1.substr(0, 9) != str[2] || s1.back() != '\"') {
        invalid();
        return;
      } else {
        string s = s1.substr(9);
        s.pop_back();
        Books_system::show('a', s, pri);
      }
    } break;
    case 'k': {
      if (s1.size() <= 11 || s1.substr(0, 10) != str[3] || s1.back() != '\"') {
        invalid();
        return;
      } else {
        string s = s1.substr(10);
        s.pop_back();
        if (s.find('|') != s.npos) {
          return invalid();
          } else {
          Books_system::show('k', s, pri);
        }
      }
    } break;
    default: {
      return invalid();    
    }
    }
  }
}
int main(int argc, char *argv[]) {
  clock_t start, end;
  start = clock(); // 开始时间
  // if(argc) freopen(argv[1],"r",stdin);
  // freopen("test.out","w",stdout);

  std::string s;
  Accounts_system::Init();
  Books_system::Init();
  Log_system::Init();
  while (getline(cin, s)) {
    static int a = 0;
    int pri = Accounts_system::get_pri();
    if (!onlySpaces(s)) {
      invalid();
      continue;
    }
    std::istringstream stream(s);
    std::string s1;
    if (!(stream >> s1))
      continue;
    if (s1 == "quit" || s1 == "exit") {
      if (stream >> s1) {
        invalid();
        continue;
      }
      goto retu;
    } else if (s1 == "su" || s1 == "register" || s1 == "passwd" ||
               s1 == "useradd" || s1 == "delete" || s1 == "logout")
      Accounts_system::read(stream, s1[0], pri);
    else if (s1 == "buy" || s1 == "select" || s1 == "modify" || s1 == "import")
      Books_system::read(stream, s1[0], pri);
    else if (s1 == "report") {
      if (pri != 7) {
        invalid();
        continue;
      }
      string ss = s1;
      stream >> s1;
      Log_system::read(s1, ss[0]);
    } else 
    if (s1 == "show")
      show(stream, s1, pri);
    else {
      if (s == "log")
        Log_system::Log(stream, pri);
      else {
        invalid();
      }
    }
  }
retu:;
  Accounts_system::stack::select(0);
  Accounts_system::end();
  Books_system::end();
  Log_system::end();
  end = clock(); // 结束时间
  std::cerr << "time = " << double(end - start) / CLOCKS_PER_SEC << "s"
            << std::endl;
}