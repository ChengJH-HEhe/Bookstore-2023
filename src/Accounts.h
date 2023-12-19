#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include "Invalid.h"
#include "map.hpp"
#include <cctype>
#include <sstream>
#include <vector>
using std::string;
namespace Accounts_system {

/*stack 设计
  当前用户与 op select int的匹配
*/
namespace Accounts {

struct Account {
  char UserID[31] = "", Password[31] = "", Username[31] = "";
  int Pri = -1, sta = 0;

  Account(char *a = nullptr, char *b = nullptr, char *c = nullptr) {
    strcpy(UserID, a);
    strcpy(Password, b);
    strcpy(Username, c);
  }
}; // Add a semicolon here

inline bool pd_loose(string s) {
  if (s.size() > 30)
    return false;
  for (int i = 0; i < s.size(); ++i)
    if (std::iscntrl(s[i]))
      return false;
  return true;
}

inline bool pd(string s) {
  if (s.size() > 30)
    return false;
  for (int i = 0; i < s.size(); ++i)
    if (!isdigit(s[i]) && !isalpha(s[i]) && s[i] != '_')
      return false;
  return true;
}

int Find_id(const char *s);
Account Find_accounts(int id);
void modify_account(int id, Account now);
void registerUser(Account, int pri = 1);
void passwd(Account, int, char *newPass);
void deleteUser(char *a, int id);
void accountsInit();
void logout();
} // namespace Accounts

namespace wait {

void init();
void update();
int getid();
void getback(int id);

}; // namespace wait

namespace stack {
struct mystack {
  int book = 0, id = 0, pri = -1;
  mystack(int b = 0, int d = 0, int p = 0) {
    book = b, id = d;
    pri = p;
  }
  
};
} // namespace stack

int get_pri();

void read(std::istringstream &, char, int);
void Init();

} // namespace Accounts_system
/*
  summary
  1.给定的是char*，如何找到对应数据？
  映射到int之后，char* int形成对，在文件存储系统中更新
  2.栈中存登录状态，操作栈怎么存？
  存成vector, 读的时候先读参数，写的时候转成数组再写

*/

#endif