#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "Ngram.h"

using namespace std;
/**************************
*      global var         *
**************************/
int ngram_order = 2;
Vocab voc;
Ngram lm(voc, ngram_order);

/**************************
*      helper func        *
**************************/
char HI(int wc)
{
  return (wc >> 8) & 255;
}

char LO(int wc)
{
  return wc & 255;
}

double getProb(string s1, string s2)
{
  VocabIndex wid = voc.getIndex(s1.c_str());
  if (wid == Vocab_None) return -1e5;
  VocabIndex context[] = { voc.getIndex(s2.c_str()), Vocab_None };
  return lm.wordProb( wid, context);
}
/*****************************************
*                 MAIN                   *
*****************************************/
int main(int argc, char* argv[])
{
  assert(argc == 3);
  ifstream file, map;
  ofstream ofs;

  file.open(argv[1], ifstream::in);
  map.open("ZhuYin-Big5.map", ifstream::in);
  ofs.open(argv[2], ofstream::out);

  if(!file.is_open()) cerr << "Error opening input file!\n";

  {
    const char lm_filename[] = "./bigram.lm";
    File lmFile(lm_filename, "r");
    lm.read(lmFile);
    lmFile.close();
  };

  string buf;
  std::map<int, vector<int>> allWord;
  while (getline(map, buf)) { // put all words in a map
    int pos = 0;

    auto yield1big5 = [&pos, &buf] () -> int {
      int wc = 0;
      while (pos < buf.size()) {
        if (buf[pos] != ' ' && buf[pos] != '\n') break;
        ++pos;
      }
      if (pos >= buf.size()) return 0;
      wc = buf[pos] & 255;
      wc <<= 8;
      wc |= buf[pos+1] & 255;
      pos += 2;
      return wc;
    };
    int key = yield1big5();
    while (true) {
      int hc = yield1big5();
      if (hc == 0) break;
      allWord[key].push_back(hc);
    }
  }

  while (getline(file, buf)) { // start running viterbi per sentence
    int pos = 0;

    auto yield1big5 = [&pos, &buf] () -> int {
      int wc = 0;
      while (pos < buf.size()) {
        if (buf[pos] != ' ' && buf[pos] != '\n') break;
        ++pos;
      }
      if (pos >= buf.size()) return 0;
      wc = buf[pos] & 255;
      wc <<= 8;
      wc |= buf[pos+1] & 255;
      pos += 2;
      return wc;
    };

    int counter = 1;
    vector<vector<double>> LPtable;
    vector<vector<string>> Wtable;
    vector<vector<int>> traceTable;

    while (true) { // read each word
      int word2 = yield1big5();
      if (word2 != 0) {
        vector<double> tempP;  LPtable.push_back(tempP);
        vector<string> tempW;  Wtable.push_back(tempW);
        vector<int> tempT;     traceTable.push_back(tempT);
      }
      if (word2 == 0) { // last word
        for (size_t i = 0; i < Wtable[counter-2].size(); ++i) {
          string before = Wtable[counter-2][i];
          LPtable[counter-2][i] += getProb("</s>", before);
        }
        break;
      }
      for (size_t i = 0; i < allWord[word2].size(); ++i) {
        string str;
        str.push_back(HI(allWord[word2][i]));
        str.push_back(LO(allWord[word2][i]));
        if (counter == 1) { // first word
          LPtable[counter-1].push_back(getProb(str, "<s>"));
          traceTable[counter-1].push_back(0);
        }
        else if (counter > 1 && word2 != 0) { // not last word
          string before = Wtable[counter-2][0];
          double max = getProb(str, before) + LPtable[counter-2][0];
          int trace = 0;
          for (size_t j = 1; j < Wtable[counter-2].size(); ++j) { // find max prob
            string before = Wtable[counter-2][j];
            double prob = getProb(str, before) + LPtable[counter-2][j];
            if (prob > max) { max = prob ; trace = j; }
          }
          LPtable[counter-1].push_back(max);
          traceTable[counter-1].push_back(trace);
        }
        Wtable[counter-1].push_back(str);
      }
      ++counter;
    }

    double MM = LPtable[counter-2][0];
    int p = 0;
    for (size_t i = 1; i < LPtable[counter-2].size(); ++i) { // find viterbi_path start point
      double tt = LPtable[counter-2][i];
      if (tt > MM) { MM = tt; p = i; }
    }

    vector<int> viterbi_path;
    for (int i = traceTable.size()-1; i >= 0; --i) {
      viterbi_path.push_back(p);
      p = traceTable[i][p];
    }

    ofs << "<s> ";
    for (int i = 0; i < Wtable.size(); ++i) {
      ofs << Wtable[i][viterbi_path[Wtable.size()-i-1]] << " ";
    }
    ofs << "</s>" << endl;

  }
  return 0;
}

