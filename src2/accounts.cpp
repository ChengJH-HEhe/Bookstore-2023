#include "accounts.h"
#include "dynamic.h"
#include "invalid.h"
#include <cassert>
#include <cstdio>
#include <fstream> // Include the missing header file
#include <vector>
using std::vector;

namespace Accounts_system {

int idcount = 0;

/* stack */
namespace stack {
std::vector<mystack> vector_st;
fstream stack;
void Init() {
  // 登录栈大小， new一个数组转成vector
  // Blocksize 读取
  stack.open("stack");
  if (!stack.good()) {
    stack.open("stack", std::ios::out);
    stack.close();
    // 依赖 acMap
    Accounts::accountsInit();
  } else {
    stack.close();
    int stacksize;
    stack.read(reinterpret_cast<char *>(&stacksize), sizeof(int));
    mystack *st = new mystack[stacksize];
    stack.read(reinterpret_cast<char *>(st), stacksize * sizeof(mystack));
    for (int i = 0; i < stacksize; ++i)
      vector_st.push_back(st[i]);
    delete[] st;
  }
}

void end() {
  stack.open("stack");
  int stacksize = vector_st.size();
  stack.write(reinterpret_cast<char *>(&stacksize), sizeof(int));
  if (stacksize) {
    mystack *st = new mystack[stacksize];
    // std::cerr<<st<<std::endl;
    for (int i = 0; i < stacksize; ++i)
      st[i] = vector_st[i];
    stack.write(reinterpret_cast<char *>(st), stacksize * sizeof(mystack));
    delete[] st;
  }
  stack.close();
}
void select(int bookid) {
  if (!bookid) {
    while (vector_st.size()) {
      Accounts::Account x;
      int id = vector_st.back().id;
      Accounts::Find_accounts(x, id);
      if (x.sta) {
        x.sta = 0;
        Accounts::modify_account(id, x);
      }
      vector_st.pop_back();
    }
    return;
  }
  if (vector_st.empty()) {
    return invalid();
  }
  vector_st.back().book = bookid;
}
mystack back() {
  if (vector_st.empty())
    return mystack();
  return vector_st.back();
}
} // namespace stack
namespace Accounts {

Map acMap;
fstream account;
Dynamic queue("accounts");
void accountsInit() {
  // create admin
  static string def[2] = {"root", "sjtu"};
  Accounts::registerUser(Accounts::Account(const_cast<char *>(def[0].c_str()),
                                           const_cast<char *>(def[1].c_str())),
                         7);
}
int Find_id(const char *s) {
  // User ID
  // std::cerr<<s<<" "<<acMap.find(s)<<std::endl,
  return acMap.find(s);
}
void Find_accounts(Account &nw, int id) {
  account.open("account");
  assert(account.is_open());
  account.seekg((id - 1) * sizeof(Account));
  account.read(reinterpret_cast<char *>(&nw), sizeof(Account));
  account.close();
}
void logout() {
  if (stack::vector_st.empty())
    return invalid();
  else {
    Account now;
    Find_accounts(now, stack::vector_st.back().id);
    now.sta--;
    modify_account(stack::vector_st.back().id, now);
    stack::vector_st.pop_back();
  }
}

void modify_account(int id, Account now) {
  account.open("account");
  account.seekp((id - 1) * sizeof(Account));
  account.write(reinterpret_cast<char *>(&now), sizeof(Account));
  //std::cerr<< id <<" "<< now.Password<<" "<<now.UserID<<" "<<now.Username<<" "<<now.sta<<" "<<"数字？"<<std::endl;
  account.close();
}

void registerUser(Account a, int pri) {
  a.Pri = pri;
  if (~Find_id(a.UserID))
    return invalid();
  int id = queue.getid();
  acMap.ins(a.UserID, id);
  modify_account(id, a);
}

void deleteUser(char *a, int id) {
  queue.getback(id);
  acMap.remove(acMap.getinfo(a, id));
}

} // namespace Accounts

int get_pri() {
  if (stack::vector_st.empty())
    return 0;
  return stack::vector_st.back().pri;
}

void read(std::istringstream &stream, char tp, int su_pri) {
  using namespace Accounts;
  string s[5] = {"@", "@", "@", "@", "@"};
  int sz = 0;
  while (stream >> s[sz++])
    ;
  --sz;
  // for(int i = 0; i < sz; ++i) std::cout<<s[i]<<" ";
  // std::cout<<std::endl;
  switch (tp) {
  case 's': {
    // 保证@是合法 控制符
    if(sz > 2 || sz < 1) return;
    int id = Find_id(s[0].c_str());
    if (id == -1)
      return invalid();
    Account candidate;
    Find_accounts(candidate, id);
    // std::cout << "candidaate" << id << " " << candidate.Password <<std::endl;
    if (!pd(s[0]) || (sz == 2 && !pd(s[1])))
      return invalid();
    if (sz == 1) {
      if (su_pri <= candidate.Pri)
        return invalid();
      else
        candidate.sta++;
    } else {
      if (strcmp(candidate.Password, s[1].c_str()))
        return 
               invalid(); // std::cerr<<candidate.Password << " "
                          // <<s[1].c_str(), invalid();
      else
        candidate.sta++;
    }
    // std::cout<<1;
    stack::vector_st.push_back(stack::mystack(0, id, candidate.Pri));
    modify_account(id, candidate);
  } break;
  case 'r': {
    if (sz != 3 || !pd(s[0]) || !pd(s[1]) || !pd_loose(s[2]))
      return invalid();
    if (~Find_id(s[0].c_str()))
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
    if (id == -1)
      return invalid();
    Account now;
    Find_accounts(now, id);
    //std::cerr << id << " " << now.Password << " " << now.UserID << std::endl;
    if (su_pri < 1 || (sz == 3 && strcmp(now.Password, s[1].c_str())) ||
        (sz == 2 && su_pri != 7))
      return invalid();
    if (sz == 3)
      strcpy(now.Password, s[2].c_str());
    else if (sz == 2)
      strcpy(now.Password, s[1].c_str());
    modify_account(id, now);
  } break;
  case 'u': {
    // std::cout<<sz<<std::endl;
    if (sz != 4 || !pd(s[0]) || !pd(s[1]) || !pd_loose(s[3]))
      return invalid();
    if (s[2] != "7" && s[2] != "3" && s[2] != "1" && s[2] != "0")
      return invalid();
    if (su_pri <= s[2][0] - '0')
      return invalid();
    registerUser(Account(const_cast<char *>(s[0].c_str()),
                         const_cast<char *>(s[1].c_str()),
                         const_cast<char *>(s[3].c_str())),
                 s[2][0] - '0');
  } break;
  case 'd': {
    if(sz>1) return invalid();
    if (su_pri != 7 || !pd(s[0]))
      return invalid();
    int id = Find_id(s[0].c_str());
    Account nw;
    Find_accounts(nw, id);
    if (id == -1 || nw.sta)
      return invalid();
    deleteUser(const_cast<char *>(s[0].c_str()), id);
  } break;
  case 'l': {
    if(sz)return invalid();
    logout();
  } break;
  default: {
  } break;
  }
}

void Init() {
  Accounts::account.open("account");
  if (!Accounts::account.good())
    Accounts::account.open("account", std::ios::out); // std::cerr<<"out\n";
  Accounts::account.close();

  Accounts::queue.init();
  Accounts::acMap.init("BlockAC", "NodeAC");
  stack::Init();
  // assert(0);
}
void end() {
  stack::end();
  Accounts::acMap.end();
  Accounts::queue.end();
}
} // namespace Accounts_system
