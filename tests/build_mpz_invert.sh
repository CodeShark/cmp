#!/bin/bash
gcc -o mpz_invert mpz_invert.c ../src/cmp_z.c ../src/cmp.c ../src/cmp_rand.c -I../src -lgmp -Wall -O3
