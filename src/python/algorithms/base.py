from __future__ import annotations

from dataclasses import dataclass
from typing import Protocol

import numpy as np


@dataclass
class CompressionResult:
    compressed_samples: np.ndarray
    labels: np.ndarray
    centers: np.ndarray
    compactness: float


class Compressor(Protocol):
    def compress(self, samples: np.ndarray, clusters: int) -> CompressionResult:
        ...

