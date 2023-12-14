#ifndef ACCOUNTS_H
#define ACCOUNTS_H

namespace Accounts_system {
  class Accounts {
    char UserID[30], Password[30], Username[30];
    int Privilege;
    public:
    void su(char* ID, char* Pass);
  }; // Add a semicolon here
  void logout();
  void registerUser(char* ID, char* Pass, char* User, int pri = 1);
  void passwd(char* ID, char* Pass, char* newPass);
  void deleteUser(char* ID);
}
/*
  summary 
  1.给定的是char*，如何找到对应数据？ 
  映射到int之后，char* int形成对，在文件存储系统中更新
  2.栈中存登录状态，操作栈怎么存？
  存成vector, 读的时候先读参数，写的时候转成数组再写

*/

#endif