#!/usr/bin/env bash

for i in *.tst
do
    # echo "/etapa6 < ${i%.tst}.tst > ${i%.tst}.iloc"
    # echo "gcc -g3 -o3 $i -o ${i%.c}.out"
    # gcc -S "$i"
    # ./etapa6 < ${i%.tst}.tst > ${i%.tst}.iloc
    ./etapa6 < "$i" > ${i%.tst}.iloc
    # echo "./etapa6 < "$i" > ${i%.tst}.iloc"
done