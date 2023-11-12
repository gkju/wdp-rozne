#!/bin/bash

mkdir myout
mkdir stefanout

for i in {0..999}
do
    ./sol < ./tests/trz_$i.in > ./myout/trz_$i.out
    ./brute < ./tests/trz_$i.in > ./stefanout/trz_$i.out
    diff -w ./myout/trz_$i.out ./stefanout/trz_$i.out || break
done