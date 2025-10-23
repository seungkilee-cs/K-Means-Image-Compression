from __future__ import annotations

import numpy as np

from .base import CompressionResult, Compressor
from .. import lloyd


class KMeansCompressor(Compressor):
    def __init__(self, max_iter: int = 100, tolerance: float = 1e-4) -> None:
        self._max_iter = max_iter
        self._tolerance = tolerance

    def compress(self, samples: np.ndarray, clusters: int) -> CompressionResult:
        if samples.dtype != np.float32:
            raise ValueError("samples must be float32")

        centers, labels = lloyd.kmeans(samples, clusters, self._max_iter, self._tolerance)
        compressed = centers[labels]
        compactness = np.sum(np.square(samples - compressed), dtype=np.float64)

        return CompressionResult(
            compressed_samples=compressed.astype(np.float32, copy=False),
            labels=labels,
            centers=centers,
            compactness=float(compactness),
        )

