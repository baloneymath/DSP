#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include <map>

using namespace std;

#define MAX_LEN 128

int main(int argc, char* argv[])
{
  assert(argc == 2);
  char* output = argv[1];
  ifstream ifs;
  ofstream ofs;
  ifs.open("Big5-ZhuYin.map", ifstream::in);

  if (!ifs.is_open()) cerr << "Error opening file!!\n";

  map<char, char> ZhuYinmap;
  map<char, vector<char>> target;
  // mapping
  string buf;
  while (getline(ifs, buf)) {
    size_t pos = buf.find_first_of(" /");
    while (pos != string::npos) {
      ZhuYinmap.emplace(buf[pos+1], buf[0]);
    }
  }
  // constructing new table
  map<char, char>::iterator it;
  for (it = ZhuYinmap.begin(); it != ZhuYinmap.end(); ++it) {
    if(!target.find(it->first)) {
      vector<char> bucket;
      bucket.push_back(it->second);
      target.emplace(it->first, bucket);
    }
    else {
      target.find(it->first)->second.push_back(it->second);
    }
  }

  ofs.open(output, ofstream::out);

  return 0;
}
