import { Samples } from "../preprocessing.js";

export interface CompressionResult {
  compressed: Samples;
  labels: Uint32Array;
  centers: Float32Array;
  compactness: number;
}

export interface KMeansConfig {
  maxIterations?: number;
  tolerance?: number;
}

export class KMeans {
  private readonly maxIterations: number;
  private readonly tolerance: number;

  constructor({ maxIterations = 100, tolerance = 1e-4 }: KMeansConfig = {}) {
    this.maxIterations = maxIterations;
    this.tolerance = tolerance;
  }

  compress(samples: Samples, clusters: number): CompressionResult {
    if (samples.cols !== 3) {
      throw new Error("samples must have 3 columns");
    }
    if (clusters < 2 || clusters > 256) {
      throw new Error("clusters must be in [2, 256]");
    }
    if (samples.rows < clusters) {
      throw new Error("sample count must be >= clusters");
    }

    const centers = this.initializeCenters(samples, clusters);
    const labels = new Uint32Array(samples.rows);

    for (let iter = 0; iter < this.maxIterations; iter += 1) {
      this.assignClusters(samples, centers, labels);
      const newCenters = this.updateCenters(samples, labels, clusters);

      if (this.hasConverged(centers, newCenters)) {
        centers.set(newCenters);
        break;
      }

      centers.set(newCenters);
    }

    this.assignClusters(samples, centers, labels);
    const compressed = this.buildCompressedSamples(samples, centers, labels);
    const compactness = this.computeCompactness(samples, compressed);

    return {
      compressed,
      labels,
      centers,
      compactness,
    };
  }

  private initializeCenters(samples: Samples, clusters: number): Float32Array {
    const centers = new Float32Array(clusters * samples.cols);
    const indices = shuffle(range(samples.rows));

    for (let i = 0; i < clusters; i += 1) {
      const sampleIdx = indices[i] * samples.cols;
      const centerIdx = i * samples.cols;
      centers[centerIdx] = samples.data[sampleIdx];
      centers[centerIdx + 1] = samples.data[sampleIdx + 1];
      centers[centerIdx + 2] = samples.data[sampleIdx + 2];
    }

    return centers;
  }

  private assignClusters(samples: Samples, centers: Float32Array, labels: Uint32Array): void {
    const { rows, cols } = samples;
    const clusterCount = centers.length / cols;

    for (let i = 0; i < rows; i += 1) {
      const sampleIdx = i * cols;
      let bestCluster = 0;
      let bestDist = Number.POSITIVE_INFINITY;

      for (let c = 0; c < clusterCount; c += 1) {
        const centerIdx = c * cols;
        const dist = squaredDistance(samples.data, centers, sampleIdx, centerIdx);

        if (dist < bestDist) {
          bestDist = dist;
          bestCluster = c;
        }
      }

      labels[i] = bestCluster;
    }
  }

  private updateCenters(samples: Samples, labels: Uint32Array, clusters: number): Float32Array {
    const { rows, cols } = samples;
    const centers = new Float32Array(clusters * cols);
    const counts = new Uint32Array(clusters);

    for (let i = 0; i < rows; i += 1) {
      const cluster = labels[i];
      const sampleIdx = i * cols;
      const centerIdx = cluster * cols;
      centers[centerIdx] += samples.data[sampleIdx];
      centers[centerIdx + 1] += samples.data[sampleIdx + 1];
      centers[centerIdx + 2] += samples.data[sampleIdx + 2];
      counts[cluster] += 1;
    }

    for (let c = 0; c < clusters; c += 1) {
      const count = counts[c];
      const centerIdx = c * cols;
      if (count > 0) {
        centers[centerIdx] /= count;
        centers[centerIdx + 1] /= count;
        centers[centerIdx + 2] /= count;
      }
    }

    return centers;
  }

  private hasConverged(oldCenters: Float32Array, newCenters: Float32Array): boolean {
    let maxShift = 0;
    for (let i = 0; i < oldCenters.length; i += 3) {
      const shift = Math.sqrt(
        squaredDistance(oldCenters, newCenters, i, i),
      );
      if (shift > maxShift) {
        maxShift = shift;
      }
    }
    return maxShift <= this.tolerance;
  }

  private buildCompressedSamples(
    samples: Samples,
    centers: Float32Array,
    labels: Uint32Array,
  ): Samples {
    const { rows, cols } = samples;
    const data = new Float32Array(rows * cols);

    for (let i = 0; i < rows; i += 1) {
      const cluster = labels[i];
      const sampleIdx = i * cols;
      const centerIdx = cluster * cols;
      data[sampleIdx] = centers[centerIdx];
      data[sampleIdx + 1] = centers[centerIdx + 1];
      data[sampleIdx + 2] = centers[centerIdx + 2];
    }

    return { data, rows, cols };
  }

  private computeCompactness(original: Samples, compressed: Samples): number {
    let sum = 0;
    for (let i = 0; i < original.data.length; i += 1) {
      const diff = original.data[i] - compressed.data[i];
      sum += diff * diff;
    }
    return sum;
  }
}

function squaredDistance(
  a: Float32Array,
  b: Float32Array,
  aIdx: number,
  bIdx: number,
): number {
  const dx = a[aIdx] - b[bIdx];
  const dy = a[aIdx + 1] - b[bIdx + 1];
  const dz = a[aIdx + 2] - b[bIdx + 2];
  return dx * dx + dy * dy + dz * dz;
}

function range(count: number): number[] {
  return Array.from({ length: count }, (_, idx) => idx);
}

function shuffle<T>(array: T[]): T[] {
  const result = array.slice();
  for (let i = result.length - 1; i > 0; i -= 1) {
    const j = Math.floor(Math.random() * (i + 1));
    [result[i], result[j]] = [result[j], result[i]];
  }
  return result;
}
