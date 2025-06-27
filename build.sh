#!/bin/bash

if [ ! -d build ]; then
    echo "No build directory. Please run the configure script first!" 1>&2
    exit 1
fi

if [ ! -f build/build.ninja ]; then
    echo "Missing ninja build script. Please make sure to run the configure script first and that you are using the Ninja generator" 1>&2
    exit 1
fi

if [ ! command -v ninja >/dev/null 2>&1 ]; then
    echo "Ninja is not installed. Please install it to build this." 1>&2
    exit 1
fi

cd build
ninja


