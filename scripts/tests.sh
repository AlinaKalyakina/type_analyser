#!/bin/bash
OUT=./analyser.out
EXE=./exe/
RES=./results/
TESTS=./tests/
RIGHT=./right_ans/
VAL_RES=./valgrind_output
mkdir -p $RES
mkdir -p $TESTS
rm -f $RES/$VAL_RES
for VAR in " " "\n \t\n" "i+j+k" "1+j*ai[65555555555555555555555555555555555555555555555555555555555555548]" "f"  "fst"  "fijkaasai" "i    + \n (k+1)" "\tas\t" "i\n\n+  j" "aai[i][j]*ai[j]+5" "aa+aa" "k+\n\t   faas" "f5as" "faf?" "fa5jk" "i+ i5s98" "5i+1" "6+56[i]+6" "fjfk(" "ff" "ai[j]+at[k]"  "3++5" "4+\nai[i" "(1+2]" "+i" "1+" "3+5)" "5@1" "i+ai[j][k]" "  as[s]+5" "afasasais+5" "5*fai" "laast"
do
echo $VAR
echo -n -e $VAR>$TESTS/${VAR// /_}
valgrind --quiet --leak-check=full $EXE/$OUT<$TESTS/${VAR// /_} 1>$RES/${VAR// /_} 2>>$RES/$VAL_RES
diff -q -b -Z -B $RES/${VAR// /_} $RIGHT/${VAR// /_}
done
echo -n -e "f" >$TESTS/testmain
$EXE/testmain.out >$RES/testmain  <$TESTS/testmain

