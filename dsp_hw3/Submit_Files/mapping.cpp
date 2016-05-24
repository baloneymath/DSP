#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

char HI(int wc)
{
  return (wc >> 8) & 255;
}

char LO(int wc)
{
  return wc & 255;
}

int main(int argc, char* argv[])
{
  assert(argc == 3);
  char* output = argv[2];
  ifstream ifs;
  ofstream ofs;
  ifs.open(argv[1], ifstream::in);
  ofs.open(argv[2], ofstream::out);

  if (!ifs.is_open()) cerr << "Error opening file!!\n";

  map<int, vector<int>> target;
  vector<int> allWord;
  // mapping
  string buf;
  while (getline(ifs, buf)) {
    int pos = 0;

    auto yield1big5 = [&pos, &buf] () -> int {
      int wc = 0;
      while (pos < buf.size()) {
        if (buf[pos] != ' ' && buf[pos] != '\n') break;
        ++pos;
      }
      if (pos >= buf.size()) return 0;
      switch (buf[pos]) {
        case '/':
          wc = '/';
          break;
        default:
          wc = buf[pos] & 255;
          wc = wc << 8;
          wc |= buf[pos+1] & 255;
          ++pos;
      }
      ++pos;
      return wc;
    };

    int key = yield1big5();
    while (true) {
      int hc = yield1big5();
      target[hc].push_back(key);
      while (hc = yield1big5()) {
        if (hc == '/') break;
      }
      if (hc == 0) break;
    }
  }
  for (auto it = target.begin(); it != target.end(); ++it) {
    ofs << HI(it->first) << LO(it->first) << " ";
    vector<int> &vec = it->second;
    sort(vec.begin(), vec.end());
    vec.resize(unique(vec.begin(), vec.end()) - vec.begin());
    for (auto wc : it->second) {
      ofs << " " << HI(wc) << LO(wc);
      allWord.push_back(wc);
    }
    ofs << endl;
  }
  sort(allWord.begin(), allWord.end());
  allWord.resize(unique(allWord.begin(), allWord.end()) - allWord.begin());
  for (auto i : allWord) {
    ofs << HI(i) << LO(i) << "  " << HI(i) << LO(i) << endl;
  }

  return 0;
}
