#ifndef LOG_H
#define LOG_H
#include <sstream>
#include "invalid.h"
namespace Log_system {
  
  void add(double);
  void read(std::istringstream &stream, char c = '\0', int pri = 0);
  void Init();
  void Log();
  void end();
}
#endif