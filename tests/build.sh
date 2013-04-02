#!/bin/bash
gcc -o op_correctness op_correctness.c ../src/cmp.c -I../src -Wall -O3
gcc -o op_performance op_performance.c ../src/cmp.c ../src/cmp_rand.c -I../src -Wall -O3
gcc -o gmp_performance gmp_performance.c ../src/cmp.c ../src/cmp_rand.c -I../src -lgmp -Wall -O3
