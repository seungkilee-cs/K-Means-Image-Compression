# TypeScript Image Compression Module

## Overview
- **Entry Point**: `src/typescript/src/index.ts`
- **CLI**: `src/typescript/src/cli.ts`
- **Image I/O**: `src/typescript/src/io.ts`
- **Preprocessing**: `src/typescript/src/preprocessing.ts`
- **Algorithms**: `src/typescript/src/algorithms/kmeans.ts`
- **Timing**: `src/typescript/src/timing.ts`
- **Pipeline**: `src/typescript/src/pipeline.ts`

## Setup
```bash
cd src/typescript
npm install
```

## Build
```bash
npm run build
```

## Run
```bash
npm run dev -- <input.png> <k> <outputDir> [--jpeg-quality Q]
# or
npm run build
npm start -- <input.png> <k> <outputDir> [--jpeg-quality Q]
```
