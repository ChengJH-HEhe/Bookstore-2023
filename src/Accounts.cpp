#include "Accounts.h"
#include "Invalid.h"
#include <cstdio>
#include <vector>

using std::vector;

namespace Accounts_system {
Map acMap;

int idcount = 0;

int stacksize = 0;

struct mystack{
  int book = -1, id = -1;
};
vector<mystack> st;

void Init() { 
  acMap.init("BlockAC", "NodeAC");

}
int current_pri() { return acMap.first(); }

void read(std::istringstream &stream, char tp, int su_pri) {
  /*
   "s" login
   "r" register
   "p" modify password
   "u" useradd
   "d" delete
   "l" logout
  */
  string s[5] = {"@", "@", "@", "@", "@"};
  int sz = 0;
  while (stream >> s[sz++]);
  // 读入切片

  switch (tp) {
  case 's': {
    // 保证@是合法 控制符
    int id = Find_id(s[0].c_str());
    Accounts candidate = Find_accounts(id);
    if (!pd(s[0]) || (sz == 2 && !pd(s[1])))
      return invalid();
    if (sz == 1) {
      if (su_pri > candidate.Pri)
        return invalid();
      else
        candidate.sta ++;
    } else {
      if (strcmp(candidate.Password, s[1].c_str()))
        return invalid();
      else
        candidate.sta ++;
    }
    modify_account(id, candidate);
  } break;
  case 'r': {
    if (sz != 3 || !pd(s[0]) || !pd(s[1]) || !pd_loose(s[2]))
      return invalid();
    registerUser(Accounts(const_cast<char *>(s[0].c_str()),
                          const_cast<char *>(s[1].c_str()),
                          const_cast<char *>(s[2].c_str())),
                 1);
  } break;
  case 'p': {
    if (sz != 2 && sz != 3)
      return invalid();
    for (int i = 0; i < sz; ++i)
      if (!pd(s[i]))
        return invalid();
    int id = Find_id(s[0].c_str());
    Accounts now = Find_accounts(id);
    if (now.Pri == -1 || (sz == 3 && strcmp(now.Password, s[2].c_str())) ||
        (sz == 2 && su_pri != 7))
      return invalid();
    strcpy(now.Password, s[1].c_str());
    modify_account(id, now);
  } break;
  case 'u': {
    if (sz != 4 || !pd(s[0]) || !pd(s[1]) || !pd_loose(s[3]))
      return invalid();
    if (s[2] != "7" && s[2] != "3" && s[2] != "1" && s[2] != "0")
      return invalid();
    if (su_pri <= s[2][0] - '0')
      return invalid();
    registerUser(Accounts(const_cast<char *>(s[0].c_str()),
                          const_cast<char *>(s[1].c_str()),
                          const_cast<char *>(s[2].c_str())),
                 s[2][0] - '0');
  } break;
  case 'd': {
    if (su_pri != 7 || !pd(s[0]))
      return invalid();
    deleteUser(const_cast<char*>(s[0].c_str()),Find_id(s[0].c_str()));
  } break;
  case 'l': {
    logout();
  } break;
  }
}

int Find_id(char* s);
Accounts Find_accounts(int id);
void modify_account(int id, Accounts now);

void logout() {
  if (!stacksize)
    return invalid();
  else {
    Accounts now = Find_accounts(st[stacksize-1].id);
    now.sta --;
    modify_account(st[--stacksize].id, now);
    st.pop_back();
  }
}
void registerUser(Accounts, int pri){

}
void passwd(Accounts, int, char *newPass){

}
void deleteUser(char* a, int id){

}


} // namespace Accounts_system
