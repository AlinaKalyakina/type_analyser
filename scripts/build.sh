#!/bin/bash
SRC=./src/
INC=./inc/
EXE=./exe/
cd $EXE
g++-7 --coverage -Wall -std=gnu++1z ../$SRC/*.cpp -o ./analyser.out -I ../$INC
