./train 1 model_init.txt ../seq_model_01.txt model_01.txt
./train 1 model_init.txt ../seq_model_02.txt model_02.txt
./train 1 model_init.txt ../seq_model_03.txt model_03.txt
./train 1 model_init.txt ../seq_model_04.txt model_04.txt
./train 1 model_init.txt ../seq_model_05.txt model_05.txt
./test modellist.txt ../testing_data1.txt result1.txt
echo -n "1,"
./acc result1.txt ../testing_answer.txt acc.txt
echo ""
echo -n "1," >> training_result.csv
./acc result1.txt ../testing_answer.txt acc.txt >> training_result.csv
echo "" >> training_result.csv

i=2
while [ "$i" != "2001" ]
do
  cp model_01.txt model_01_init.txt
  cp model_02.txt model_02_init.txt
  cp model_03.txt model_03_init.txt
  cp model_04.txt model_04_init.txt
  cp model_05.txt model_05_init.txt
  ./train 1 model_01_init.txt ../seq_model_01.txt model_01.txt
  ./train 1 model_02_init.txt ../seq_model_02.txt model_02.txt
  ./train 1 model_03_init.txt ../seq_model_03.txt model_03.txt
  ./train 1 model_04_init.txt ../seq_model_04.txt model_04.txt
  ./train 1 model_05_init.txt ../seq_model_05.txt model_05.txt
  ./test modellist.txt ../testing_data1.txt result1.txt
   echo -n "$i,"
  ./acc result1.txt ../testing_answer.txt acc.txt
  echo -n "$i," >> training_result.csv
  ./acc result1.txt ../testing_answer.txt acc.txt >> training_result.csv
  echo "" >> training_result.csv
  echo \ 
  i=$(($i+1))
done
echo "DONE!"
