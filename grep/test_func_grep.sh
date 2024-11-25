#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"qwerty vsem.privet VAR"
"qwerty vsem.privet 1.text VAR"
"-e qwerty -e privet vsem.privet 1.text VAR"
"-e qwerty -e v vsem.privet VAR"
"-e v -e qwerty vsem.privet VAR -f privet.txt"
"-e qwerty -e v vsem.privet VAR -f privet.txt"
)

declare -a extra=(
"-n qwerty vsem.privet 1.text"
"-n qwerty vsem.privet"
"-n -e ^\} vsem.privet"
"-c -e '/\' vsem.privet"
"-ce ^qwerty vsem.privet 1.text"
"-e ^qwerty vsem.privet"
"-nivh = vsem.privet 1.text"
"-e"
"-ie qwertY 1.text"
"-eqwerty vsem.privet 1.text"
"-ne qwerty -e v 1.text"
"-iv qwerty 1.text"
"-in qweRty 1.text"
"-c -l qwerty vsem.privet 1.text"
"-noe ) 1.text"
"-l for vsem.privet 1.text"
"-e a -e A 1.text"
"-noe ing -e as -e the -e not -e is 1.text"
"-e ing -e as -e the -e not -e is 1.text"
"-c -e . vsem.privet -e '.'"
"-l for no_file.txt 1.text"
"-f privet.txt 1.text"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $t"
    else
      (( FAIL++ ))
      echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $t"
    fi
    rm test_s21_grep.log test_sys_grep.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in v c l n h o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# 3 параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        for var3 in v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1 -$var2 -$var3"
                    testing $i
                done
            fi
        done
    done
done

# 2 сдвоенных параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

# 3 строенных параметра
for var1 in v c l n h o
do
    for var2 in v c l n h o
    do
        for var3 in v c l n h o
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="-$var1$var2$var3"
                    testing $i
                done
            fi
        done
    done
done

echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"