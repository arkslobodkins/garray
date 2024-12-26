#! /bin/bash

ls
mode=$1

if [ "$mode" == "" ]; then
   mode="0"
fi

if [ "$mode" == 0 ]; then
exe0="example0.x"
exe1="example1.x"
exe2="example2.x"
exe3="example3.x"
exe4="example4.x"
exe5="example5.x"
exe6="example6.x"
else
exe0="example0_debug.x"
exe1="example1_debug.x"
exe2="example2_debug.x"
exe3="example3_debug.x"
exe4="example4_debug.x"
exe5="example5_debug.x"
exe6="example6_debug.x"
fi

make all debug=$mode -j8

echo -e "\nRUNNING EXAMPLE 0"
./$exe0

echo -e "\nRUNNING EXAMPLE 1"
./$exe1

echo -e "\nRUNNING EXAMPLE 2"
./$exe2

echo -e "\nRUNNING EXAMPLE 3"
./$exe3

echo -e "\nRUNNING EXAMPLE 4"
./$exe4

echo -e "\nRUNNING EXAMPLE 5"
./$exe5

echo -e "\nRUNNING EXAMPLE 6"
./$exe6

echo -e "\nCLEAN EXAMPLES"
make clean
echo -e ""
