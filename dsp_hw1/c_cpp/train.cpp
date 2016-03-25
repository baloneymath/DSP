#include <string.h>
#include <stdio.h>
#include <iostream>
#include <cassert>

#include "hmm.h"

using namespace std;

void
buildAlphas(double alphas[MAX_OBSERV][MAX_STATE], const HMM& hmm, const double b[MAX_STATE][MAX_OBSERV])
{
  int stateNum = hmm.state_num;
  int observNum = hmm.observ_num;
  for (int i = 0; i < stateNum; ++i)
    alphas[0][i] = hmm.initial[i] * b[i][0];
  for (int t = 1; t < observNum; ++t) {
    for (int j = 0; j < stateNum; ++j) {
      double temp = 0.0;
      for (int k = 0; k < stateNum; ++k)
        temp += hmm.transition[k][j] * alphas[t-1][k];
      alphas[t][j] = temp * b[j][t];
    }
  }
}

void
buildBetas(double betas[MAX_OBSERV][MAX_STATE], const HMM& hmm, const double b[MAX_STATE][MAX_OBSERV])
{
  int stateNum = hmm.state_num;
  int observNum = hmm.observ_num;
  for (int i = 0; i < stateNum; ++i)
    betas[observNum-1][i] = 1;
  for (int t = observNum-2; t >= 0 ; --t) {
    for (int i = 0; i < stateNum; ++i){
      double temp = 0.0;
      for (int j = 0; j < stateNum; ++j)
        temp += hmm.transition[i][j] * b[j][t+1] * betas[t+1][j];
      betas[t][i] = temp;
    }
  }
}

void
buildGammas(double gammas[MAX_OBSERV][MAX_STATE], const HMM& hmm, const double alphas[MAX_OBSERV][MAX_STATE], const double betas[MAX_OBSERV][MAX_STATE])
{
  int stateNum = hmm.state_num;
  int observNum = hmm.observ_num;
  for (int t = 0; t < observNum; ++t) {
    double temp_q = 0.0;
    for (int k = 0; k < stateNum; ++k)
      temp_q += alphas[t][k] * betas[t][k];
    for (int i = 0; i < stateNum; ++i) {
      gammas[t][i] = alphas[t][i] * betas[t][i] / temp_q;
    }
  }

}

void
buildEpsilons(
    double epsilons[MAX_OBSERV][MAX_STATE][MAX_STATE],
    const HMM& hmm,
    const double alphas[MAX_OBSERV][MAX_STATE],
    const double betas[MAX_OBSERV][MAX_STATE],
    const double b[MAX_STATE][MAX_OBSERV]
    )
{
  int stateNum = hmm.state_num;
  int observNum = hmm.observ_num;
  for (int t = 0; t < observNum-1; ++t) {
    double temp_q = 0.0;
    for (int i = 0; i < stateNum; ++i)
      for (int j = 0; j < stateNum; ++j)
        temp_q += alphas[t][i] * hmm.transition[i][j] * b[j][t+1] * betas[t+1][j];
    for (int i = 0; i < stateNum; ++i)
      for (int j = 0; j < stateNum; ++j)
        epsilons[t][i][j]
          = alphas[t][i] * hmm.transition[i][j] * b[j][t+1] * betas[t+1][j] / temp_q;
  }
}

int main(int argc, char * argv[])
{
  assert(argc == 5);
  int iter = atoi(argv[1]);
  char* model_init = argv[2];
  char* model_seq = argv[3];
  char* model_out = argv[4];
  FILE* fpseq = open_or_die(model_seq, "r");
  FILE* fp = open_or_die(model_out, "w");

  // Create init
  HMM hmm, hmm_res;
  loadHMM(&hmm, model_init);

  // alias variables
  int N = hmm.state_num;
  double accumGamma[MAX_SEQ][MAX_STATE] = {0.0};
  double accumGammaO[MAX_OBSERV][MAX_STATE] = {0.0};
  double accumEpsilon[MAX_SEQ][MAX_STATE][MAX_STATE] = {0.0};
  auto &pi = hmm.initial;
  auto &b = hmm.observation;
  auto &a = hmm.transition;

  // start training
  for (int _i = 1; _i <= iter; ++_i) {
    char o[MAX_SEQ+1];
    fscanf(fpseq, "%s", &o[1]);
    int T = strlen(&o[1]);

    double alpha[MAX_SEQ][MAX_STATE] = {0.0}; // create alpha table
    for (int i = 0; i < N; ++i) {
      alpha[1][i] = pi[i] * b[o[1] - 'A'][i];
    }
    for (int t = 1; t < T; ++t) {
      for (int j = 0; j < N; ++j) {
        double temp = 0.0;
        for (int i = 0; i < N; ++i) {
          temp += alpha[t][i] * a[i][j];
        }
        alpha[t+1][j] = temp * b[o[t+1] - 'A'][j];
      }
    }

    double beta[MAX_SEQ][MAX_STATE] = {0.0}; // create beta table
    for (int i = 0; i < N; ++i) {
      beta[T][i] = 1.0;
    }
    for (int t = T-1; t >= 1; --t) {
      for (int i = 0; i < N; ++i) {
        double temp = 0.0;
        for (int j = 0; j < N; ++j) {
          temp += a[i][j] * b[o[t+1]-'A'][j] * beta[t+1][j];
        }
        beta[t][i] = temp;
      }
    }

    double gamma[MAX_SEQ][MAX_STATE] = {0.0}; // create gamma table
    for (int t = 1; t <= T; ++t) {
      double temp = 0.0;
      for (int i = 0; i < N; ++i) {
        temp += alpha[t][i] * beta[t][i];
      }
      for (int i = 0; i < N; ++i) {
        gamma[t][i] = alpha[t][i] * beta[t][i] / temp;
      }
    }

    double epsilon[MAX_SEQ][MAX_STATE][MAX_STATE] = {0.0}; // create epsilon table
    for (int t = 1; t <= T-1; ++t) {
      double temp = 0.0;
      for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
          temp += alpha[t][i] * a[i][j] * b[o[t+1]-'A'][j] * beta[t+1][j];
        }
      }
      for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
          epsilon[t][i][j] = alpha[t][i] * a[i][j] * b[o[t+1]-'A'][j] * beta[t+1][j] / temp;
        }
      }
    }

    for (int t = 1; t <= T; ++t) {
      for (int i = 0; i < N; ++i) {
        accumGamma[t][i] += gamma[t][i];
        accumGammaO[o[t] - 'A'][i] += gamma[t][i];
      }
    }
    for (int t = 1; t <= T-1; ++t) {
      for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
          accumEpsilon[t][i][j] += epsilon[t][i][j];
        }
      }
    }

    for (int i = 0; i < N; ++i) {
      pi[i] = accumGamma[1][i] / _i;
    }
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        double up = 0.0, down = 0.0;
        for (int t = 1; t <= T-1; ++t) {
          up += accumEpsilon[t][i][j];
          down += accumGamma[t][i];
        }
        a[i][j] = up / down;
      }
    }

    for (int i = 0; i < N; ++i) {
      for (int k = 0; k < hmm.observ_num; ++k) {
        double up = 0.0, down = 0.0;
        for (int t = 1; t <= T; ++t) {
          down += accumGamma[t][i];
        }
        up = accumGammaO[k][i];
        b[k][i] = up / down;
      }
    }
  /*
     cerr << _i << ':' << endl;
     dumpHMM(stderr, &hmm);
     cerr << endl;
  */
  }
  dumpHMM(fp, &hmm);
  return 0;
}

