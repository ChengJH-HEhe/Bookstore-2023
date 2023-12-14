#include <cstdio>
#include "Accounts.h"
#include "Invalid.h"
#include <vector>

using std::vector;

namespace Accounts_system {
  int account;
  vector<int> st;
  void su(char* ID, char* Pass) {

  }
  void logout() {
    if(!account) return invalid();
    else --account;
  }
  void registerUser(char* ID, char* Pass, char* User, int pri);
  void passwd(char* ID, char* Pass, char* newPass);
  void deleteUser(char* ID);
}
