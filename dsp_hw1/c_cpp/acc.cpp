#include <iostream>
#include <cassert>

#include "hmm.h"

using namespace std;

int main(int argc, char* argv[])
{
  assert(argc == 4);
  char* data = argv[1];
  char* answer = argv[2];
  char* result = argv[3];

  FILE* fp = open_or_die(data, "r");
  FILE* fpans = open_or_die(answer, "r");
  FILE* fpres = open_or_die(result, "w");

  char o[MAX_SEQ];
  char ans[MAX_SEQ];

  double counter = 0.0;
  double match = 0.0;

  while (fgets(o, MAX_SEQ, fp)) {
    ++counter;
    fgets(ans, MAX_SEQ, fpans);
    if (o[7] == ans[7]) ++match;
  }
  double acc = match / counter;
  cout << acc;
  fprintf(fpres, "%lf", acc);


  return 0;
}
