#!/bin/zsh
PATH=~/Desktop/DSP/dsp_hw3
for i in 1 2 3 4 5 6 7 8 9 10
do
  echo SRILM disambig $i.txt...
  ./disambig -text testdata/$i.txt -map ./Submit_Files/ZhuYin-Big5.map -lm ./bigram.lm -order 2 > ./Submit_Files/result1/$i.txt
done
