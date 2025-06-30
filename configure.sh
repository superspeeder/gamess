#!/bin/bash

if [ ! -d build ]; then
    mkdir build
fi

cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE


