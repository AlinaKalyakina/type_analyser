#!/bin/bash
OUT=./analyser.out
EXE=./exe/
RES=./results/
TEST_RES=./test_result
VAL_RES=./valgrind_output
mkdir -p  $RES
rm -f $RES/$TEST_RES
rm -f $RES/$VAL_RES
for VAR in " \n \t" "i+j+k" "1+j*ai[6548]" "f"  "fst"  "fijkaasai" "i    + \n (k+1)" "\tas\t" "i\n\n+  j" "aai[i][j]*ai[j]+5" "aa+aa" "k+\n\t   faas" "f5as" "faf?" "fa5jk" "i+ i5s98" "5i+1" "6+56[i]+6" "fjfk(" "ff" "ai[j]+at[k]"  "3++5" "4+\nai[i" "(1+2]" "+i" "1+" "3+5)" "5@1" "i+ai[j][k]" "  as[s]+5" "afasasais+5" "5*fai"
do
echo -n -e $VAR >> $RES/$TEST_RES
echo -n -e $VAR > tmp
echo $VAR;
valgrind --quiet --leak-check=full $EXE/$OUT<tmp 1>>$RES/$TEST_RES 2>>$RES/$VAL_RES
echo -e "\n*******" >> $RES/$TEST_RES
done
rm tmp
$EXE/testmain.out >> $RES/$TEST_RES

