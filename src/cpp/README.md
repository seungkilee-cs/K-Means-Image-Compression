# C++ Image Compression Module

## Overview
- **CLI** (`src/cpp/app/main.cpp`, `src/cpp/include/cli.hpp`, `src/cpp/src/cli.cpp`)
- **Image I/O** (`src/cpp/include/image_io.hpp`, `src/cpp/src/image_io.cpp`)
- **Preprocessing** (`src/cpp/include/image_preprocessor.hpp`, `src/cpp/src/image_preprocessor.cpp`)
- **Algorithms** (`src/cpp/include/algorithms`, `src/cpp/src/algorithms`)
- **Timing** (`src/cpp/include/timing.hpp`, `src/cpp/src/timing.cpp`)

## Build & Test
```bash
mkdir -p build && cd build
cmake ..
make -j
ctest --output-on-failure
```

## Run
```bash
./kmeans_imgc <input.png> <k> <output_dir> [jpeg_quality]
```
