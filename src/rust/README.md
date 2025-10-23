# Rust Image Compression Module

## Overview
- **Entry Point**: `src/rust/src/main.rs`
- **CLI**: `src/rust/src/cli.rs`
- **Image I/O**: `src/rust/src/io.rs`
- **Preprocessing**: `src/rust/src/preprocessing.rs`
- **Algorithms**: `src/rust/src/algorithms/kmeans.rs`
- **Timing**: `src/rust/src/timing.rs`
- **Pipeline**: `src/rust/src/pipeline.rs`

## Build & Test
```bash
cd src/rust
cargo check
cargo test
```

## Run
```bash
cargo run -- <input.png> <k> <output_dir> [--jpeg-quality Q]
```
