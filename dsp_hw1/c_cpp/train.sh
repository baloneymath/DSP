#!/bin/zsh
PATH=~/Desktop/DSP/dsp_hw1/c_cpp
for x in 1 2 3 4 5
do
  echo training model_0"$x" $1 times...
  ./train  $1  model_init.txt  ../seq_model_0"$x".txt  model_0"$x".txt
done
