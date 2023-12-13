#include <cstring>
#include <fstream>
#include <iostream>
#include <math.h>
#include <queue>
using std::fstream;
using std::string;


// bool isopen;
template <class T, int info_len = 0> class MemoryRiver {
private:
  /* your code here */
  string file_name;
  fstream file;
  int sizeofT = sizeof(T);

public:
  MemoryRiver() = default;

  MemoryRiver(const string &file_name) : file_name(file_name) {}

  void initialise(string FN = "") {
    if (FN != "")
      file_name = FN;
    file.open(file_name, std::ios::out);
    int tmp;
    for (int i = 0; i < info_len; ++i)
      file.write(reinterpret_cast<char *>(&tmp), sizeof(T));
    file.close();
  }
  int write(T &t) { /* your code here */
    file.open(file_name, std::ios::in | std::ios::out);
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
  void read(T &t, const int index) { /* your code here */
    file.open(file_name, std::ios::in | std::ios::out);
    file.seekg(index);
    file.read(reinterpret_cast<char *>(&t), sizeof(T));
    file.close();
  }
};
/*
但禁止将非当前指令所需数据存储于内存中。
*/
// 取出数据就是 新建一个变量 read 然后那个变量会清空

const int N = 1e5 + 5;
const int mxn = 257;
typedef long long ll;
class info {
public:
  char *idx;
  ll value = 0;
  info(ll val = 0) {
    value = val;
    idx = nullptr;
  };
  bool operator<(const info &a) const {
    int pd = strcmp(idx, a.idx);
    return (pd < 0) || (pd == 0 && value < a.value);
  }
};
// 维护每块的第一个字符串
// now nxt
struct link {
  int now = 0, nxt = 0;
  int pos = 0;
  info first;
}lnk[1005];

struct node {
  info x[517];
  int size = 0;
  void push(info, link &);
  void check();
  void clear(info, link &, link &);
  bool find(char *);
};
MemoryRiver<node> map("map");
MemoryRiver<link [1005]> blockinfo("blockinfo");
int head,n;
// node 对应的link
// now 和 link 的 now
// void printall() {
//   link t;
//   blockinfo.read(t, head);
//   node now;
//   map.read(now, t.now);
//   std::cout<<now.size<<std::endl;
//   for(int i = 0; i < now.size; ++i)
//     std::cout<<now.x[i].idx<<" "<<now.x[i].value<<" ";
//   std::cout<<std::endl;
//   while (t.nxt) {
//     link t1 ;
//     blockinfo.read(t1, t.nxt);
//     t = t1;
//     node now;
//     map.read(now, t.now);
//     std::cout<<now.size<<std::endl;
//     for(int i = 0; i < now.size; ++i)
//       std::cout<<now.x[i].value<<" ";
//     std::cout<<std::endl;
//   }
// }
void node::push(info a, link &b) {
  int i;
  for (i = 0; i < size && x[i] < a; ++i);
  for (int j = size; j > i; --j) {
    x[j] = x[j - 1];
  }
  // 维护对应的link
  if (i == 0) {
    b.first = a;
  }
  x[i] = a;
  ++size;
  if (size > (mxn << 1)) {
    node nw;
    link nw1;
    nw1.first = x[mxn];
    for (int i = mxn; i < size; ++i)
      nw.push(x[i], nw1);
    size = mxn;
    nw1.nxt = b.nxt;
    nw1.now = map.write(nw);
    nw1.pos = b.nxt = blockinfo.write(nw1);
    blockinfo.update(b, b.pos);
    blockinfo.update(nw1, nw1.pos);

    map.update(*this, b.now);  
  }else map.update(*this, b.now);
  // if(n==1486) {
  //   std::cout<<n<<" _________________________________________________"<<std::endl;
  //   printall();
  // }
}


void node::clear(info a, link &pre, link &now) {
  int i;
  for (i = 0; i < size && x[i] < a; ++i);
  if (i == size || a < x[i] || x[i] < a)
    return;
  // std::cout<<x[i].idx<<" "<<x[i].value<<" "<<std::endl;
  for (int j = i; j < size - 1; ++j) {
    x[j] = x[j + 1];
  }
  --size;
  if (size == 0) {
    if (!pre.now) {
      head = now.nxt;
      now.now = now.nxt = 0;
    } else {
      pre.nxt = now.nxt;
      blockinfo.update(pre, pre.pos);
    }
  }
  map.update(*this, now.now);
}
bool node::find(char *idx) {
  int i;
  bool pd = 0;
  for (i = 0; i < size && strcmp(x[i].idx, idx) < 0; ++i)
    ;
  while (i < size && strcmp(x[i].idx, idx) == 0) {
    std::cout << x[i].value << " ";
    pd = 1;
    ++i;
  }
  return pd;
}
// 每一块最后一个info对应的index
// 每一块的名字
//

// 1. 读取文件，建立块状链表

void gethead() {
  map.initialise();
  blockinfo.initialise();
  node head1;
  link ttt;
  ttt.now = map.write(head1);
  ttt.pos = head = blockinfo.write(ttt);
  blockinfo.update(ttt, head);
}

int main() {
   freopen("a.txt", "r", stdin);
  freopen("a1.txt", "w", stdout);
  std::cin >> n;
  gethead();
  while (n--) {
    std::string op, s;
    std::cin >> op >> s;
    if (op == "insert") {
      ll v;
      std::cin >> v;
      info tmp(v);
      tmp.idx = new char[64];
      strcpy(tmp.idx, s.c_str());
      link t;
      blockinfo.read(t, head);
      while (t.nxt) {
        link t1;
        blockinfo.read(t1, t.nxt);
        if (t1.first < tmp)
          t = t1;
        else 
          break;
      }
      node now;
      now.size=0;
      map.read(now, t.now);
      now.push(tmp, t);
    } else if (op == "delete") {
      ll v;
      std::cin >> v;
      info tmp(v);
      tmp.idx = new char[64];
      strcpy(tmp.idx, s.c_str());

      link t, tt;
      blockinfo.read(t, head);
      while (t.nxt) {
        link now1;
        blockinfo.read(now1, t.nxt);
        if (now1.first < tmp) {
          tt = t, t = now1; // sqrt(n)次赋值sqrt(n) 复杂度n爆了。
        } else {
          break;
        }
      }
      node now;
      map.read(now, t.now);
      now.clear(tmp, tt, t);
    } else if (op == "find") {
      char *idx = new char[64];
      strcpy(idx, s.c_str());
      link t;
      blockinfo.read(t, head);
      while (t.nxt) {
        link now1;
        blockinfo.read(now1, t.nxt);
        if (strcmp(now1.first.idx, idx) < 0)
          t = now1;
        else {
          break;
        }
      }
      node now;
      map.read(now, t.now);
      bool pd = now.find(idx);
      int pos;
      while (t.nxt) {
        link now1;
        blockinfo.read(now1, t.now);
        if (strcmp(now1.first.idx, idx) == 0)
          t = now1;
        else
          break;
        map.read(now, t.now);
        pd |= now.find(idx);
      }
      if (!pd) {
        std::cout << "null" << std::endl;
      } else {
        std::cout << std::endl;
      }
    }
  }
}
