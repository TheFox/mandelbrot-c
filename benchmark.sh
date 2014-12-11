#!/usr/bin/env bash

BIN=./build_$(uname -s)/bin/mandelbrot
set +x

time $BIN 1024 1024 1 100 -.5 2 0 2 8

echo "done"
