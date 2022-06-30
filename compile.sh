#!/bin/bash

rm -rf output.txt
make

for i in $(seq 1 1000);do 
echo "SEED $i: " >> output.txt;
echo "v1: " >> output.txt;
./saida1 $i >> output.txt;
echo "v2: " >> output.txt;
./saida2 $i >> output.txt;
echo "" >> output.txt;
done;
