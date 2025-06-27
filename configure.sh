#!/bin/bash

if [ ! -d build ]; then
    mkdir build
fi

if [ ! -f vcpkg_toolchain.cmake ]; then
    if [ ! command -v vcpkg >/dev/null 2>&1 ]; then
        echo "vcpkg_toolchain.cmake file not found in root (symlink it here) and vcpkg could not be found (may not be in path)." 1>&2
        exit 1
    else
        VCPKG_PATH="$(realpath "$(command -v vcpkg)")"
        VCPKG_DIR="$(dirname "$VCPKG_PATH")"
        VCPKG_SCRIPT="$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake"
        ln -s $VCPKG_SCRIPT ./vcpkg_toolchain.cmake
        echo "Created symbolic link ./vcpkg_toolchain.cmake -> $VCPKG_SCRIPT"
    fi
fi


VCPKG_TOOLCHAIN="$(realpath "./vcpkg_toolchain.cmake")"
cd build
cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE="$VCPKG_TOOLCHAIN" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE


