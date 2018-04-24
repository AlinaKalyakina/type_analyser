#!/bin/bash
SRC=./src/
INC=./inc/
EXE=./exe/
mkdir -p $EXE
cd $EXE
for VAR in analysistree errors lexseq main synanalys typedetector testmain
do
g++-7 --coverage -Wall -O3 -fno-omit-frame-pointer -fno-optimize-sibling-calls -c -std=gnu++1z ../$SRC/$VAR.cpp -o ./$VAR.o -I ../$INC
done
g++-7 --coverage -Wall -std=gnu++1z analysistree.o errors.o lexseq.o main.o synanalys.o typedetector.o -o analyser.out -I ../$INC
g++-7 --coverage -Wall -std=gnu++1z analysistree.o errors.o lexseq.o testmain.o synanalys.o typedetector.o -o testmain.out -I ../$INC
