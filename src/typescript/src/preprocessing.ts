import type { ImageData } from "./io.js";

export interface Samples {
  data: Float32Array;
  rows: number;
  cols: number;
}

export function imageToSamples(image: ImageData): Samples {
  const { data, width, height } = image;
  const rows = width * height;
  const cols = 3;
  const samples = new Float32Array(rows * cols);

  for (let i = 0; i < rows; i += 1) {
    const srcIdx = i * 3;
    const dstIdx = i * cols;
    samples[dstIdx] = data[srcIdx];
    samples[dstIdx + 1] = data[srcIdx + 1];
    samples[dstIdx + 2] = data[srcIdx + 2];
  }

  return { data: samples, rows, cols };
}

export function samplesToImage(
  samples: Samples,
  width: number,
  height: number,
): ImageData {
  if (samples.cols !== 3) {
    throw new Error("samples must have 3 columns");
  }

  if (samples.rows !== width * height) {
    throw new Error("sample count mismatch");
  }

  const data = new Uint8Array(width * height * 3);
  const { rows, cols } = samples;

  for (let i = 0; i < rows; i += 1) {
    const srcIdx = i * cols;
    const dstIdx = i * 3;
    data[dstIdx] = clampToByte(samples.data[srcIdx]);
    data[dstIdx + 1] = clampToByte(samples.data[srcIdx + 1]);
    data[dstIdx + 2] = clampToByte(samples.data[srcIdx + 2]);
  }

  return { data, width, height };
}

function clampToByte(value: number): number {
  if (value < 0) return 0;
  if (value > 255) return 255;
  return value | 0;
}
