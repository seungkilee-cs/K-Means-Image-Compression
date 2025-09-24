# K-Means-Image-Compression

My implementation of Image Compression by K Means Clustering

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

#### Original Image (474kb/PNG)

![PNG](./assets/Lenna.png)

#### Compressed Image with 16 clusters and 90 JPEG quality (82kb/JPEG)

![JPEG](./src/cpp/out/Lenna_k16_q90.jpg)

## Python

WIP

## Typescript

WIP

## Rust

WIP

## Next Agenda

- Build TS frontend to allow real time comparison between the original image uploaded vs the compressed image.
- Build Client Side Image Compression App to upload and compress images.
- In the TS client, allow for parameter slides to see the actual compression results in real time.
- Support other compression algorithms and allow dropdown selection for the users to test out different compression algorithms
- Add metadata display for original and compressed image
- support more output formats
- Flesh out to Client side image compression app with TS UI and support the core for CLI
