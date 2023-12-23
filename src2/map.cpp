#include "map.hpp"

/*--------------__block Function-----------------*/
int __block::getid() { return pool[0] ? pool[pool[0]--] : ++id; }
void __block::getback(int nw) { pool[++pool[0]] = nw; }
// remove id1 (tp = 1) or id1.nxt (tp = 0)
void __block::remove(int id1, bool tp) {
  // std::cout<<_<<" "<<id1<<" "<<tp<<std::endl;
  if (tp) {
    if (message[id1].nxt) {
      head = message[id1].nxt;
      getback(id1);
    }
  } else {
    int id2 = message[id1].nxt, id3 = message[id2].nxt;
    message[id1].nxt = id3;
    getback(id2);
  }
}
int __block::addnew(int id1) {
  int id2 = message[id1].nxt, id3 = getid();
  message[id3].nxt = id2;
  message[id1].nxt = id3;
  return id3;
}

/*----------------Info Function-----------------------*/

bool info::operator<(const info &b) const {
  int pd = strcmp(str, b.str);
  return (pd < 0) || (pd == 0 && val < b.val);
}
bool info::operator==(const info &b) const {
  return strcmp(str, b.str) == 0 && val == b.val;
}

/*----------------Map Function------------------*/

void Map::init(string s1, string s2) {
  Block.file.open(s1);
  if (!Block.file.good()) {
    Block.initialise(s1);
    map.initialise(s2);
    string u = "";
    block.head = block.getid();
    block.message[block.head].first = info(0);
    strcpy(block.message[block.head].first.str, u.c_str());
    block.message[block.head].pos = map.write(node);
    return;
    // 判断的文件变多，这个文件的要求要升高，一旦加就要更新
  } else {
    Block.file.close();
    Block.file_name = s1;
    map.file_name = s2;
    Block.read(block,0);
    Block.file.close();
  }
}
void Map::end() {
  Block.update(block,0);
}
info Map::getinfo(const char *str, ll value) {
  info temp;
  strcpy(temp.str, str), temp.val = value;
  return temp;
}; // namespace Map
pair<int, int> Map::getpos(info a) {
  int id1 = block.head, id2 = 0, id3 = 1;
  do {
    id2 = id1;
    id1 = block.message[id1].nxt;
  } while (id1 && block.message[id1].first < a);
  if (id1 && block.message[id1].first == a)
    return std::make_pair(id1, 0);
  id1 = id2;
  map.read(node, block.message[id1].pos);
  for (int i = 0; i < node.size; ++i) {
    if (node.x[i] == a)
      return std::make_pair(id1, i);
    if (a < node.x[i])
      return std::make_pair(id1, i);
  }
  return std::make_pair(id1, node.size);
}
int Map::find(const char *a) {
  // find a pos;
  //std::cout<<a << block.head<<std::endl;
  int blockl = block.head, blockr = block.head;
  do {
    blockr = blockl;
    blockl = block.message[blockl].nxt;
  } while (blockl && strcmp(block.message[blockl].first.str, a) < 0);
  blockl = blockr;
  map.read(node, block.message[blockl].pos);
  for (int i = 0; i < node.size; ++i)
    if (!strcmp(node.x[i].str, a)) {
      return node.x[i].val;
    }
  blockr = block.message[blockr].nxt;
  if (blockr && strcmp(block.message[blockr].first.str, a) == 0)
    return block.message[blockr].first.val;
  return -1;
}


void Map::multifind(std::vector<int> &v, const char *a) {
  if(a == nullptr) {
    int nw = block.head;
    while(nw){
      map.read(node, block.message[nw].pos);
      for (int i = 0; i < node.size; ++i)
        if(node.x[i].val)v.push_back(node.x[i].val);
      nw = block.message[nw].nxt;
    }
  }
  
  int blockl = block.head, blockr = block.head;
  do {
    blockr = blockl;
    blockl = block.message[blockl].nxt;
  } while (blockl && strcmp(block.message[blockl].first.str, a) < 0);
  blockl = blockr;
  bool pd = 0;
  map.read(node, block.message[blockl].pos);
  for (int i = 0; i < node.size; ++i)
    if (!strcmp(node.x[i].str, a)) {
      v.push_back(node.x[i].val);
    }
  blockr = block.message[blockr].nxt;
  while (blockr && strcmp(block.message[blockr].first.str, a) == 0) {
    map.read(node, block.message[blockr].pos);
    for (int i = 0; i < node.size; ++i)
      if (!strcmp(node.x[i].str, a)) {
        v.push_back(node.x[i].val);
      }
    blockr = block.message[blockr].nxt;
  }
}

#define st first
#define nd second
void Map::insert(pair<int, int> pos, info a) {
  bool blockupd = (pos.nd == 0);
  // uninitialised
  if (pos.nd == 0)
    block.message[pos.st].first = a;
  if (pos.nd == node.size) {
    node.x[node.size++] = a;
    
  } else {
    for (int i = node.size; i > pos.nd; --i)
      node.x[i] = node.x[i - 1];
    node.x[pos.nd] = a;
    ++node.size;
  }

  if (node.size > (mxn * 2)) {
    __node nw;
    for (int i = mxn; i < node.size; ++i)
      nw.x[nw.size++] = node.x[i];
    node.size = mxn;
    int newid = block.addnew(pos.st);
    block.message[newid].first = nw.x[0];
    block.message[newid].pos = map.write(nw);
  }
  map.update(node, block.message[pos.st].pos);
}

void Map::remove(info a) {
  // node
  pair<int, int> pos;
  int id1 = block.head, id2 = 0, pre = 0;
  do {
    pre = id2, id2 = id1;
    id1 = block.message[id1].nxt;
  } while (id1 && block.message[id1].first < a);
  // pre id2 id1
  // 保证 ID1 在的块就是 first >= a
  pos = std::make_pair(id1, node.size);
  // id1 恰好是
  if (id1 && block.message[id1].first == a) {
    map.read(node, block.message[id1].pos);
    pos = std::make_pair(id1, 0);
    pre = id2;
  }
  // default pos.nd = node.size
  else {
    // 否则 若 id2 == head?
    id1 = id2;
    map.read(node, block.message[id1].pos);
    for (int i = 0; i < node.size; ++i) {
      if (node.x[i] == a) {
        pos = std::make_pair(id1, i);
        break;
      } else if (a < node.x[i]) {
        // assert(0);
        break;
      }
    }
  }
  if (pos.nd >= node.size || !(node.x[pos.nd] == a))
    return;
  // pos.st pos.nd 找到
  if (node.size == 1) {
    // size = 0
    --node.size;
    if (pre)
      block.remove(pre);
    else
      block.remove(pos.st, 1);
  } else {
    for (int i = pos.nd; i < node.size - 1; ++i)
      node.x[i] = node.x[i + 1];
    --node.size;
    if (!pos.nd)
      block.message[pos.st].first = node.x[0];
  }
  map.update(node, block.message[pos.st].pos);
  // now we have block[pos.first] and node[pos.first]
  // remove node[pos.first].x[pos.second]
}
void Map::printall() {
  return;
  int nw = block.head;
  do {
    map.read(node, block.message[nw].pos);
    std::cout << node.size << " Size: ";
    for (int i = 0; i < node.size; ++i)
      std::cout << node.x[i].val << " ";
    std::cout << std::endl;
    nw = block.message[nw].nxt;
  } while (nw);
}
#undef st
#undef nd

void Map::ins(char *tp, int id) {
  info tmp;
  insert(getpos(tmp = getinfo(tp, id)), tmp);
}
