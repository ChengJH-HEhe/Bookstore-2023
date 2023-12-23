#include "Log.h"
#include "invalid.h"
#include <climits>
#include <cstdio>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
namespace Log_system {
std::vector<std::pair<double, double>> history;
std::fstream log;
void add(double money) {
  std::pair<double, double> q;
  if (money < 1e-6) {
    q = std::make_pair(0.0, -money);
  } else {
    q = std::make_pair(money, 0.0);
  }
  if (history.empty()) {
    history.push_back(q);
  } else {
    history.push_back(std::make_pair(q.first + history.back().first,
                                     q.second + history.back().second));
  }
}
void read(std::istringstream &stream, char c, int pri) {
  std::string s[5];
  int sz = 0;
  while (stream >> s[sz++])
    ;
  --sz;
  if (pri != 7)
    return invalid();
  if (c == 's') {
    long long num = history.size();
    if (sz == 1) {
      long long num1 = 0;
      for (auto i = s[0].begin(); i != s[0].end(); ++i)
        if (!isdigit(*i) || num1 > INT_MAX)
          return invalid();
        else
          num1 = num1 * 10 + (*i - '0');
      num = num1;
    }
    if (num > history.size())
      return invalid();
    else if (num == history.size()) {
      std::pair<double, double> q =
          history.size() ? history.back() : std::make_pair(0.0, 0.0);
      printf("+ %.2lf - %.2lf\n", q.first, q.second);
    } else {
      std::pair<double, double> q = history[history.size() - num - 1];
      printf("+ %.2lf - %.2lf\n", history.back().first - q.first, history.back().second -  q.second);
    }
  } else {
  }
}

void Log() {}

void Init() {
  log.open("log");
  if (log.good()) {
    int tmp_size;
    log.read(reinterpret_cast<char *>(&tmp_size), sizeof(int));
    if (tmp_size) {
      double *nw = new double[tmp_size * 2];
      log.read(reinterpret_cast<char *>(nw), sizeof(nw));
      for (int i = 0; i < tmp_size; ++i)
        history.push_back(std::make_pair(nw[i * 2], nw[i * 2 + 1]));
      delete[] nw;
    }
  } else {
    log.open("log", std::ios::out);
  }
  log.close();
}
void end() {
  log.open("log");
  int sz = history.size();
  log.write(reinterpret_cast<char *>(&sz), sizeof(int));
  for (int i = 0; i < sz; ++i)
    log.write(reinterpret_cast<char *>(&history[i].first), sizeof(double)),
        log.write(reinterpret_cast<char *>(&history[i].second), sizeof(double));
  log.close();
}
} // namespace Log_system