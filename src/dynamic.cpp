#include "dynamic.h"

/*Dynamic::*/
  void Dynamic::init(){
    w.open(filename.c_str());
    if(w.good()){
      int tmp_size;
      w.read(reinterpret_cast<char*>(&idcount), sizeof(int));
      w.read(reinterpret_cast<char*>(&tmp_size), sizeof(int));
      int* nw = new int[tmp_size];
      w.read(reinterpret_cast<char*>(nw), sizeof(nw));
      for(int i = 0; i < tmp_size; ++i)
        wait.push_back(nw[i]);
      delete []nw;
    } else {
      w.open("wait", std::ios::out);
    }
    w.close();
  }
  void Dynamic::update() {
    int tmpsize = wait.size();
    int* nw = new int[tmpsize];
    for(int i = 0; i < tmpsize; ++i)
      nw[i] = wait[i];
    w.open("wait");
    w.write(reinterpret_cast<char*>(&idcount),sizeof(int));
    w.write(reinterpret_cast<char*>(&tmpsize),sizeof(int));
    w.write(reinterpret_cast<char*>(nw),tmpsize*sizeof(int));
    delete[] nw;
    w.close();
  }
  int Dynamic::getid() { return wait.size() ? wait.pop_back(), wait.back() : ++idcount; }
  void Dynamic::getback(int id) { wait.push_back(id); }