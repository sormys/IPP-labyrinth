#!/bin/bash

oCorrect=0
oWrong=0
eCorrect=0
eWrong=0
vCorrect=0
vWrong=0

for f in $2/*.in; 
do
    echo "STARTING TEST FOR $f";
    valgrind --error-exitcode=21 --leak-check=full \
             --show-leak-kinds=all --errors-for-leak-kinds=all \
             --log-fd=37 --quiet  \
             ./$1 <$f 1>test.out.tmp 2>test.err.tmp 37>valgrind.err.tmp
    valgrindResult=$?
    printf "\e[35m OUTPUT: \e[0m"
    if diff "${f%in}out"  "test.out.tmp" >/dev/null 2>&1; then 
        printf "\e[32m CORRECT  ANSWER \e[0m\n"
        oCorrect=$(($oCorrect+1))
    else 
        printf "\e[31m WRONG  ANSWER \e[0m\n"
        oWrong=$(($oWrong+1))
    fi;

    printf "\e[34m ERROR: \e[0m"
    if diff "${f%in}err"  "test.err.tmp" >/dev/null 2>&1; then 
        printf "\e[32m CORRECT  ANSWER \e[0m\n"
        eCorrect=$(($eCorrect+1))
    else 
        printf "\e[31m WRONG ANSWER \e[0m\n"
        eWrong=$(($eWrong+1))
    fi;

    printf "\e[36m MEMORY: \e[0m"
    if (($valgrindResult == 15)); then 
        printf "\e[31m WRONG ANSWER \e[0m\n"
        vWrong=$(($vWrong+1))
    else 
        printf "\e[32m CORRECT  ANSWER \e[0m\n"
        vCorrect=$(($vCorrect+1))
    fi;
    
done;
echo "SUMMARY:"
printf "OUTPUT: CORRECT: \e[32m$oCorrect\e[0m WRONG: \e[31m$oWrong\e[0m\n"
printf "ERROR: CORRECT: \e[32m$eCorrect\e[0m WRONG: \e[31m$eWrong\e[0m\n"
printf "MEMORY: CORRECT: \e[32m$vCorrect\e[0m WRONG: \e[31m$vWrong\e[0m\n\n"
echo "Cleaning temporary files..."
rm test.out.tmp test.err.tmp valgrind.err.tmp
