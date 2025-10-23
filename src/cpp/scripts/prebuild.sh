#!/usr/bin/env sh
set -eu

# Move to the directory containing this script (project root)
cd "$(dirname "$0")"

# Clean previous build
rm -rf build

# Configure
cmake -S . -B build

# Build
cmake --build build --config Release

# test k means compressions
# ./build/kmeans_imgc ../asset/Lenna.png 16 out 90
