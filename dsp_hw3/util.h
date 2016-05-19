#include <string>
#include <ctype.h>
#include <cstring>
#include <cassert>

using namespace std;

size_t
myStrGetTok(const string& str, string& tok, size_t pos = 0,
    const char del = ' ')
{
  size_t begin = str.find_first_not_of(del, pos);
  if (begin == string::npos) { tok = ""; return begin; }
  size_t end = str.find_first_of(del, begin);
  tok = str.substr(begin, end - begin);
  return end;
}

void
lex(vector<string> tokens, string option)
{
  string token;
  size_t n = myStrGetTok(option, token);
  while (token.size()) {
    tokens.push_back(token);
    n = myStrGetTok(option, token, n);
  }
}
