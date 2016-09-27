#!/bin/bash
make clean && make
mkdir -p $1
cp example.in $1/ALmodel.in
cp similar.proto $1/ALmodel.proto
./generate_run 0 $1 ALmodel 0
