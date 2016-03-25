#include "hmm.h"

void
buildAlphas(double alphas[MAX_OBSERVE][MAX_STATE], const HMM& hmm, const double b[MAX_STATE][MAX_OBSERVE])
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
buildBetas(double betas[MAX_OBSERVE][MAX_STATE], const HMM& hmm, const double b[MAX_STATE][MAX_OBSERVE])
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
buildGammas(double gammas[MAX_OBSERVE][MAX_STATE], const HMM& hmm, const alphas[MAX_OBSERVE][MAX_STATE], const betas[MAX_OBSERVE][MAX_STATE])
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
    double epsilons[MAX_OBSERVE][MAX_STATE][MAX_STATE],
    const HMM& hmm,
    const double alphas[MAX_OBSERVE][MAX_STATE],
    const double betas[MAX_OBSERVE][MAX_STATE],
    const double b[MAX_STATE][MAX_OBSERVE]
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

int main()
{
  // Create init
  HMM hmms[5];
  load_models("modellist.txt", hmms, 5);
  dump_models(hmms, 5);

  for (int i = 0; i < 5; ++i) { // For each hmm model
    int stateNum = hmms[i].state_num;
    int observNum = hmms[i].observ_num;

    double b[MAX_STATE][MAX_OBSERVE] = {0.0};
    for (int j = 0; j < stateNum; ++j)
      for (int k = 0; k < observNum; ++k) // calculate each b_j(o)
        b[j][k] += hmms[i].observation[k][j];

    double alphas[MAX_OBSERVE][MAX_STATE] = {0.0};
    buildAlphas(alphas, hmms[i], b); // create alpha table

    double betas[MAX_OBSERVE][MAX_STATE] = {0.0};
    buildBetas(betas, hmms[i], b); // create beta table

    double gammas[MAX_OBSERVE][MAX_STATE] = {0.0};
    buildGammas(gammas, hmms[i], alphas, betas); // create gamma table


    /* $\epsilon_t (i,j)$ */
    double epsilons[MAX_OBSERVE][MAX_STATE][MAX_STATE] = {0.0};
    buildEpsilons(epsilons, hmms[i], alphas, betas, b);
  }

}

