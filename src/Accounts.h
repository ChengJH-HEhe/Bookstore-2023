#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <cctype>
#include <sstream>
#include "map.hpp"
#include"Invalid.h"
using std::string;
namespace Accounts_system {

/*stack 设计
  当前用户与 op select int的匹配
*/




struct Accounts {
  char UserID[31] = "", Password[31] = "", Username[31] = "";
  int Pri=-1, sta = 0;
  
  Accounts(char* a = nullptr, char* b = nullptr, char* c = nullptr) {
    strcpy(UserID , a);
    strcpy(Password, b);
    strcpy(Username, c);
  }
}; // Add a semicolon here

void Init();
int current_pri();
void logout();

void registerUser(Accounts, int pri = 1);
void passwd(Accounts, int, char *newPass);
void deleteUser(char* a, int id);

int Find_id(const char* s);
Accounts Find_accounts(int id);
void modify_account(int id, Accounts now);

inline bool pd_loose( string s) {
  if(s.size() > 30) return false;
  for(int i = 0; i < s.size(); ++i)
    if(std::iscntrl(s[i]))
      return false;
  return true;
}

inline bool pd( string s) {
  if(s.size()>30) return false;
  for(int i = 0; i < s.size(); ++i)
    if(!isdigit(s[i]) && !isalpha(s[i]) && s[i]!='_')
      return false;
  return true;
}

  void read(std::istringstream&, char, int);
} // namespace Accounts_system
/*
  summary
  1.给定的是char*，如何找到对应数据？
  映射到int之后，char* int形成对，在文件存储系统中更新
  2.栈中存登录状态，操作栈怎么存？
  存成vector, 读的时候先读参数，写的时候转成数组再写

*/

#endif