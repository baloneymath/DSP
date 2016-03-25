#include <iostream>
#include <math.h>
#include <cassert>

#include "hmm.h"


int main(int argc, char* argv[])
{
  assert(argc == 4);
  char* model_list = argv[1];
  char* test_data = argv[2];
  char* result = argv[3];
  FILE* fptest = open_or_die(test_data, "r");
  FILE* fp = open_or_die(result, "w");

  // load models
  HMM hmms[5];
  load_models(model_list, hmms, 5);

  // start testing
  while (1) {
    char o[MAX_SEQ+1];
    if (fscanf(fptest, "%s", &o[1]) == EOF) break;
    int T = strlen(&o[1]);

    double delta[5][MAX_SEQ][MAX_STATE] = {0.0};
    for (int _i = 0; _i < 5; ++_i) {
      // alias variables
      int N = hmms[_i].state_num;
      auto &pi = hmms[_i].initial;
      auto &b = hmms[_i].observation;
      auto &a = hmms[_i].transition;

      for (int i = 0; i < N; ++i) // initialize delta
        delta[_i][1][i] = pi[i] * b[o[1] - 'A'][i];
      for (int t = 2; t <= T; ++t) { // create delta table
        for (int j = 0; j < N; ++j) {
          double temp = 0.0;
          for (int i = 0; i < N; ++i)
            if (delta[_i][t-1][i] * a[i][j] > temp)
              temp = delta[_i][t-1][i] * a[i][j];
          delta[_i][t][j] = temp * b[o[t] - 'A'][j];
        }
      }
    }

    double termination[5] = {0.0}; // P* in each model

    for (int _i = 0; _i < 5; ++_i) {
      double temp = 0.0;
      for (int i = 0; i < hmms[_i].state_num; ++i)
        if (delta[_i][T][i] > temp) temp = delta[_i][T][i];
      termination[_i] = temp;
    }

    size_t resultNum = 0;
    for (int _i = 1; _i < 5; ++_i)
      if (termination[_i] > termination[resultNum]) resultNum = _i;
    fprintf(fp, "model0%d_.txt ", resultNum);
    fprintf(fp, "%lf\n", termination[resultNum]);

  }

  dump_models(hmms, 5);

  return 0;
}
