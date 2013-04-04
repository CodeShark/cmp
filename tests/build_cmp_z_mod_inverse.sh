#!/bin/bash
gcc -o cmp_z_mod_inverse cmp_z_mod_inverse.c ../src/cmp.c ../src/cmp_z.c ../src/cmp_rand.c -I../src -Wall -O3
