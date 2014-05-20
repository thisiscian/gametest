#!/usr/bin/bash
[[ $1 == "clean" ]] && rm -r build 2</dev/null
mkdir build 2>/dev/null || exit 1; 
cd build || exit 2;
cmake .. || exit 3;
make || exit 4;
