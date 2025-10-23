from __future__ import annotations

import numpy as np

from . import image_io


def to_samples(bgr8u: np.ndarray) -> np.ndarray:
    if bgr8u.dtype != np.uint8:
        raise ValueError("to_samples expects dtype=uint8")
    if bgr8u.ndim != 3 or bgr8u.shape[2] != 3:
        raise ValueError("to_samples expects shape (H, W, 3)")
    return image_io.flatten_image(bgr8u)


def samples_to_image(samples32f: np.ndarray, rows: int, cols: int) -> np.ndarray:
    if samples32f.dtype != np.float32:
        raise ValueError("samples_to_image expects dtype=float32")
    return image_io.unflatten_image(samples32f, rows, cols)

