#!/usr/bin/bash

mkdir build 2>/dev/null || exit 1; 
cd build || exit 2;
cmake .. || exit 3;
make || exit 4;
