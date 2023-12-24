#ifndef LOG_H
#define LOG_H
#include <sstream>
#include "invalid.h"
namespace Log_system {
  
  void add(double);
  void read(std::string, char c = '\0', int pri = 0);
  void Init();
  void Log(std::istringstream&, int);
  void end();
}
#endif