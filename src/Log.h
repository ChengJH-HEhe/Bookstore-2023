#ifndef LOG_H
#define LOG_H
#include <sstream>

namespace Log_system {
  void add(double money);
  void read(std::istringstream &stream, char c = '\0');
  void Init();
}
#endif