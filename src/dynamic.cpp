#include "dynamic.h"

/*Dynamic::*/
void Dynamic::init() {
  w.open(filename.c_str());
  if (w.good()) {
    int tmp_size;
    w.read(reinterpret_cast<char *>(&idcount), sizeof(int));
    w.read(reinterpret_cast<char *>(&tmp_size), sizeof(int));
    if (tmp_size) {
      int *nw = new int[tmp_size];
      w.read(reinterpret_cast<char *>(nw), sizeof(nw));
      for (int i = 0; i < tmp_size; ++i)
        wait.push_back(nw[i]);
      delete[] nw;
    }
  } else {
    w.open(filename.c_str(), std::ios::out);
    idcount = 0;
  }
  w.close();
}
void Dynamic::end() {
  int tmpsize = wait.size();
  w.open("wait");
  w.write(reinterpret_cast<char *>(&idcount), sizeof(int));
  w.write(reinterpret_cast<char *>(&tmpsize), sizeof(int));
  if (tmpsize) {
    int *nw = new int[tmpsize];
    for (int i = 0; i < tmpsize; ++i)
      nw[i] = wait[i];
    w.write(reinterpret_cast<char *>(nw), tmpsize * sizeof(int));
    delete[] nw;
  }
  w.close();
}
int Dynamic::getid() {
  if (wait.empty())
    return ++idcount;
  int nw = wait.back();
  wait.pop_back();
  return nw;
}
void Dynamic::getback(int id) { wait.push_back(id); }