#include "accounts.h"
#include <cstdio>
#include <vector>
#include <fstream> // Include the missing header file
using std::vector;

namespace Accounts_system {

int idcount = 0;
int stacksize = 1;
namespace wait {
  
  std::fstream w; // Fix the code by including the missing header file
  std::vector<int> wait;
  void init(){

    w.open("wait");
    if(w.good()){
      int tmp_size;
      w.read(reinterpret_cast<char*>(&tmp_size), sizeof(int));
      int* nw = new int[tmp_size];
      w.read(reinterpret_cast<char*>(nw), sizeof(nw));
      for(int i = 0; i < tmp_size; ++i)
        wait.push_back(nw[i]);
      delete []nw;
    } else {
      w.open("wait", std::ios::out);
    }
    w.close();
  }
  void update() {
    int tmpsize = wait.size();
    int* nw = new int[tmpsize];
    for(int i = 0; i < tmpsize; ++i)
      nw[i] = wait[i];
    w.open("wait");
    w.write(reinterpret_cast<char*>(&tmpsize),sizeof(int));
    w.write(reinterpret_cast<char*>(nw),tmpsize*sizeof(int));
    delete[] nw;
    w.close();
  }
  int getid() { return wait.size() ? wait.pop_back(), wait.back() : ++idcount; }
  void getback(int id) { wait.push_back(id); }
}
namespace stack {
std::vector<mystack> vector_st;
  fstream stack;

void Init() {
  // 登录栈大小， new一个数组转成vector
  // Blocksize 读取
  if (!stack.good()) {
    stack.open("stack", std::ios::out);
    Accounts::accountsInit();
    vector_st.push_back(mystack(0,1));
    stack.close();
  } else {
    stack.open("stack");
    stack.seekg(0);
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

}
namespace Accounts {

Map acMap;
fstream account;

void accountsInit() {
  // create admin
  acMap.init("BlockAC", "NodeAC");
  string def[2] = {"root", "sjtu"};
  Accounts::registerUser(
    Accounts::Account(const_cast<char*>(def[0].c_str()),const_cast<char*>(def[1].c_str())), 7);
}
int Find_id(char *s) {
  // User ID
  int id = acMap.find(s);
  if(~id) {
    return id;
  }
  else {
    return wait::getid();
  }
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
  Account nw;
  account.open("account");
  if(!account.good()) {
    account.open("account",std::ios::out);
  }
  account.seekg((id-1)*sizeof(Account));
  account.write(reinterpret_cast<char*>(&nw), sizeof(Account));
}

void registerUser(Account a, int pri) {
  a.Pri = pri;
  int id = wait::getid();
  acMap.ins(a.UserID, id);
  modify_account(id, a);
}

void deleteUser(char *a, int id) {
  wait::getback(id);
  acMap.remove(acMap.getinfo(a, id));
}

}

int get_pri() {
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
    deleteUser(const_cast<char *>(s[0].c_str()), Find_id(s[0].c_str()));
  } break;
  case 'l': {
    logout();
  } break;
  }
}


} // namespace Accounts_system
