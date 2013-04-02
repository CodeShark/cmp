#!/bin/bash
gcc -o op_correctness op_correctness.c ../src/cmp.c -I../src -Wall
