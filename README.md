# K-Means-Image-Compression

My implementation of Image Compression by K Means Clustering, in various languages

## Cpp

### How To Use

First, clone this repository. Then, navigate to `src/cpp/`.

#### Prebuild

```bash
# clean previous build is there are any
rm -rf build
# confiugure and build
cmake -S . -B build
cmake --build build --config Release
```

#### Build

```bash
./build/kmeans_imgc {YOUR_TEST_IMAGE_PATH}.png {K_CLUSETERS: [2,256]} out {JPEG_QUALTIY: [0,100]}
```

Will create the poc compression for the image.

### Results

Of course, none other than the first lady of the internet

| Original (474kb/PNG)       | Compressed Image w/ 16 Clusters (82kb/JPEG) |
| -------------------------- | ------------------------------------------- |
| ![PNG](./assets/Lenna.png) | ![JPEG](./src/cpp/out/Lenna_k16_q90.jpg)    |

## Python

See `src/python/README.md` for architecture and usage details.

## Rust

See `src/rust/README.md` for architecture and usage details.

## TypeScript

See `src/typescript/README.md` for architecture and usage details.

## Typescript

Building a client-side image compressor. The problem with "free" online converters is that your data ends up on someone else's server where they can do whatever they want with it. TypeScript with Vite is my preferred tool for this kind of web application, and I want to test how well these algorithms perform in the browser.

## Rust

I've been avoiding Rust for a while, partly due to a "just write in C" mentality and partly because of my general aversion to evangelists. However, my [first experience](https://github.com/seungkilee-cs/spektra/blob/master/docs/fft-js_rust%2Bwasm_comparison.md) with Rust through WebAssembly was surprisingly positive and measurably faster than native JavaScript. After that Rust conversion therapy, I want to properly learn it as a viable tool. The best way to learn something new is to solve a problem you already understand, letting you focus on implementation rather than figuring out the problem itself.

## Next Agenda

- Modularize the C++ code. There was too much Stack Overflow involved and proper refactoring would help.
- Add compression performance comparisons.
- Build TypeScript frontend for real-time comparison between original and compressed images.
- Create client-side image compression app for uploading and compressing images.
- Add parameter sliders in the TypeScript client to see compression results in real time.
- Support additional compression algorithms with dropdown selection for users to test different approaches.
- Display metadata for both original and compressed images.
- Support more output formats.
- Expand the TypeScript UI into a full client-side image compression app while maintaining CLI support.
