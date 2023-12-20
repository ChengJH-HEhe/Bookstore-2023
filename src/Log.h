#ifndef LOG_H
#define LOG_H
#include <sstream>

namespace Log_system {

  void read(std::istringstream &stream, char c = '\0');
  void Init();
}
#endif