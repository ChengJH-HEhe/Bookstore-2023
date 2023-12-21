#include "accounts.h"
#include "dynamic.h"
#include <cstdio>
#include <vector>
#include <fstream> // Include the missing header file
using std::vector;

namespace Accounts_system {

int idcount = 0;
int stacksize = 1;

/* stack */
namespace stack {
  std::vector<mystack> vector_st;
  fstream stack;
void Init() {
  // 登录栈大小， new一个数组转成vector
  // Blocksize 读取
  if (!stack.good()) {
    stack.open("stack", std::ios::out);
    Accounts::accountsInit();
    stack.close();
  } else {
    stack.open("stack");
    stack.read(reinterpret_cast<char *>(&stacksize), sizeof(int));
    mystack* st = new mystack[stacksize];
    stack.seekg(sizeof(int));
    stack.read(reinterpret_cast<char *>(st), stacksize * sizeof(mystack));
    for(int i = 0; i < stacksize; ++i)
      vector_st.push_back(st[i]);
    delete[] st;
    stack.close();
  }
}

void end() {
  stack.open("stack");
  stack.write(reinterpret_cast<char *>(&stacksize), sizeof(int));
  mystack* st = new mystack[stacksize];
  for(int i = 0; i < stacksize; ++i)
    st[i] = vector_st[i];
  stack.write(reinterpret_cast<char *>(st), stacksize * sizeof(mystack));
  delete []st;
  stack.close();
}
void select(int bookid) {
  if(vector_st.empty())
    return invalid();
  vector_st.back().book = bookid;
}
mystack back() {
  if(vector_st.empty()) return mystack();
  return vector_st.back();
}
}
namespace Accounts {

Map acMap;
fstream account;
Dynamic queue("accounts");
void accountsInit() {
  // create admin
  acMap.init("BlockAC", "NodeAC");
  string def[2] = {"root", "sjtu"};
  Accounts::registerUser(
    Accounts::Account(const_cast<char*>(def[0].c_str()),const_cast<char*>(def[1].c_str())), 7);
}
int Find_id(const char *s) {
  // User ID
  int id = acMap.find(s);
  if(~id) {
    return id;
  }
  return -1;
}
Account Find_accounts(int id){
  Account nw;
  account.open("account");
  account.seekg((id-1)*sizeof(Account));
  account.read(reinterpret_cast<char*>(&nw), sizeof(Account));
  account.close();
  return nw;
}
void logout() {
  if (!stacksize)
    return invalid();
  else {
    Account now = Find_accounts(stack::vector_st.back().id);
    modify_account(stack::vector_st[--stacksize].id, now);
    stack::vector_st.pop_back();
  }
}

void modify_account(int id, Account now) {
  account.open("account");
  if(!account.good()) {
    account.open("account",std::ios::out);
  }
  account.seekg((id-1)*sizeof(Account));
  account.write(reinterpret_cast<char*>(&now), sizeof(Account));
  account.close();
}

void registerUser(Account a, int pri) {
  a.Pri = pri;
  int id = queue.getid();
  acMap.ins(a.UserID, id);
  modify_account(id, a);
}

void deleteUser(char *a, int id) {
  queue.getback(id);
  acMap.remove(acMap.getinfo(a, id));
}

}

int get_pri() {
  if(stack::vector_st.empty()) return -1;
  return stack::vector_st.back().pri;
}

void read(std::istringstream &stream, char tp, int su_pri) {
  using namespace Accounts;
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
  while (stream >> s[sz++])
    ;
  // 读入切片

  switch (tp) {
  case 's': {
    // 保证@是合法 控制符
    int id = Find_id(s[0].c_str());
    if(id == -1) return invalid();
    Account candidate = Find_accounts(id);
    if (!pd(s[0]) || (sz == 2 && !pd(s[1])))
      return invalid();
    if (sz == 1) {
      if (su_pri > candidate.Pri)
        return invalid();
      else
        candidate.sta++;
    } else {
      if (strcmp(candidate.Password, s[1].c_str()))
        return invalid();
      else
        candidate.sta++;
    }
    stack::vector_st.push_back(stack::mystack(0,id,candidate.Pri));
    modify_account(id, candidate);
  } break;
  case 'r': {
    if (sz != 3 || !pd(s[0]) || !pd(s[1]) || !pd_loose(s[2]))
      return invalid();
    registerUser(Account(const_cast<char *>(s[0].c_str()),
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
    if(id == -1) return invalid();
    Account now = Find_accounts(id);
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
    registerUser(Account(const_cast<char *>(s[0].c_str()),
                          const_cast<char *>(s[1].c_str()),
                          const_cast<char *>(s[2].c_str())),
                 s[2][0] - '0');
  } break;
  case 'd': {
    if (su_pri != 7 || !pd(s[0]))
      return invalid();
    int id = Find_id(s[0].c_str());
    if(id == -1) return invalid();
    deleteUser(const_cast<char *>(s[0].c_str()), id);
  } break;
  case 'l': {
    logout();
  } break;
  default: {
    stack::select(0);
  }
  }
}


} // namespace Accounts_system
