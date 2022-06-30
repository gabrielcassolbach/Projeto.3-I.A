#!/bin/bash

rm -rf output.txt
make

for i in $(seq 1 100);
do ./saida $i >> output.txt;
done;
