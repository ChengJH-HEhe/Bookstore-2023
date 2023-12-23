#ifndef dynamic
#define dynamic

/*
  一维动态数组开点 + 当前数据总量 预留第一个参数是数据总量 第二个参数是开点队列总量
*/

#include<fstream>
#include<vector>
#include<string>

inline bool pd_loose(std::string s) {
  if (s.size() > 30)
    return false;
  for (int i = 0; i < s.size(); ++i)
    if (std::iscntrl(s[i]))
      return false;
  return true; 
}

inline bool pd(std::string s) {
  if (s.size() > 30)
    return false;
  for (int i = 0; i < s.size(); ++i)
    if (!isdigit(s[i]) && !isalpha(s[i]) && s[i] != '_')
      return false;
  return true;
}
class Dynamic {
  std::fstream w; // Fix the code by including the missing header file
  std::string filename;
  std::vector<int> wait;
  public:
  Dynamic(std::string name) {
    filename = name;
  }
  int idcount = 0;
  void init();
  void end();
  int getid();
  void getback(int);
}; // namespace wait

#endif
