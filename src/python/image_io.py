from __future__ import annotations

import os
from pathlib import Path

import numpy as np
from PIL import Image


def load_color_image(path: str | os.PathLike[str]) -> Image.Image:
    image = Image.open(path).convert("RGB")
    if image.width == 0 or image.height == 0:
        raise ValueError(f"empty image: {path}")
    return image


def ensure_directory(directory: str | os.PathLike[str]) -> None:
    Path(directory).mkdir(parents=True, exist_ok=True)


def make_output_path(
    input_path: str | os.PathLike[str],
    output_dir: str | os.PathLike[str],
    clusters: int,
    quality: int,
) -> Path:
    stem = Path(input_path).stem
    filename = f"{stem}_k{clusters}_q{quality}.jpg"
    return Path(output_dir) / filename


def save_jpeg(path: str | os.PathLike[str], image: Image.Image, quality: int) -> None:
    image.save(path, format="JPEG", quality=quality, optimize=True)


def image_to_array(image: Image.Image) -> np.ndarray:
    return np.asarray(image, dtype=np.uint8)


def array_to_image(array: np.ndarray) -> Image.Image:
    if array.dtype != np.uint8:
        raise ValueError("array_to_image expects dtype=uint8")
    if array.ndim != 3 or array.shape[2] != 3:
        raise ValueError("array_to_image expects shape (H, W, 3)")
    return Image.fromarray(array, mode="RGB")


def flatten_image(array: np.ndarray) -> np.ndarray:
    if array.ndim != 3 or array.shape[2] != 3:
        raise ValueError("flatten_image expects shape (H, W, 3)")
    return array.reshape(-1, 3).astype(np.float32)


def unflatten_image(samples: np.ndarray, rows: int, cols: int) -> np.ndarray:
    if samples.ndim != 2 or samples.shape[1] != 3:
        raise ValueError("unflatten_image expects shape (N, 3)")
    if samples.shape[0] != rows * cols:
        raise ValueError("sample count does not match rows * cols")
    reshaped = samples.reshape(rows, cols, 3)
    return np.clip(reshaped, 0, 255).astype(np.uint8)

