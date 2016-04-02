#!/bin/zsh
PATH=~/Desktop/DSP/dsp_hw1/c_cpp

echo testing data"$1"...
./test modellist.txt ../testing_data"$1".txt result"$1".txt
echo calculating accuracy...
./acc result"$1".txt ../testing_answer.txt acc.txt
