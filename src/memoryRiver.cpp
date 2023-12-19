#include<fstream>
#include<cstring>

using std::string, std::fstream;

template <class T, int info_len = 2> class MemoryRiver {
  friend struct Map;
private:
  string file_name;
  fstream file;
  int sizeofT = sizeof(T);
public:
  MemoryRiver() = default;
  bool good() {
    return file.good();
  }
  MemoryRiver(const string &file_name) : file_name(file_name) {}
  
  void get_info(int &tmp, int n) {
    if (n > info_len)
      return;
    file.open(file_name);
    file.seekg((n - 1) * sizeof(int));
    file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
    file.close();
  }
  
  void write_info(int tmp, int n) {
    if (n > info_len)
      return;
    file.open(file_name, std::ios::out | std::ios::in);
    file.seekp((n - 1) * sizeof(int));
    file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    file.close();
    /* your code here */
  }
  void initialise(string FN = "") {
    if (FN != "")
      file_name = FN;
    file.open(file_name,std::ios::out);
    int tmp = 0;
    for(int i = 0; i < info_len; ++i) {
      file.write(reinterpret_cast<char*>(tmp), sizeof(int));
    }
    file.close();
  }
  int write(T &t) { /* your code here */
    file.open(file_name, std::ios::app);
    int wh = file.tellp();
    file.seekp(wh);
    file.write(reinterpret_cast<char *>(&t), sizeof(T));
    file.close();
    return wh;
  }
  // 用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
  void update(T &t, const int index) { /* your code here */
    file.open(file_name, std::ios::in | std::ios::out);
    file.seekp(index);
    file.write(reinterpret_cast<char *>(&t), sizeof(T));
    file.close();
  }

  // 读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
  void read(int t, const int index) { /* your code here */
    file.open(file_name, std::ios::in | std::ios::out);
    file.seekg(index);
    file.read(reinterpret_cast<char *>(&t), sizeof(int));
    file.close();
  }
  void read(T &t, const int index) { /* your code here */
    file.open(file_name, std::ios::in | std::ios::out);
    file.seekg(index);
    file.read(reinterpret_cast<char *>(&t), sizeof(T));
    file.close();
  }
};