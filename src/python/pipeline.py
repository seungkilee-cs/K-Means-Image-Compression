from __future__ import annotations

from pathlib import Path
from typing import Protocol

import sys

from . import cli, image_io, preprocessing, timing
from .algorithms.kmeans import KMeansCompressor


class Console(Protocol):
    def info(self, message: str) -> None: ...
    def error(self, message: str) -> None: ...


class StdConsole:
    def info(self, message: str) -> None:
        print(message)

    def error(self, message: str) -> None:
        print(message, file=sys.stderr)


def run(options: cli.Options, console: Console | None = None) -> Path:
    console = console or StdConsole()

    console.info(f"[INFO] Input: {options.input_path}")
    console.info(
        f"[INFO] k={options.clusters} out_dir={options.output_dir} quality={options.jpeg_quality}"
    )

    stopwatch = timing.Stopwatch()

    image = image_io.load_color_image(options.input_path)
    console.info(f"[INFO] Loaded {image.width}x{image.height}")
    stopwatch.tick("load")

    array = image_io.image_to_array(image)
    samples = preprocessing.to_samples(array)
    console.info(f"[INFO] Samples: {samples.shape[0]} x {samples.shape[1]}")
    stopwatch.tick("prep")

    compressor = KMeansCompressor()
    console.info("[INFO] Running kmeans...")
    result = compressor.compress(samples, options.clusters)
    console.info(f"[INFO] Kmeans done. Compactness={result.compactness:.2f}")
    stopwatch.tick("kmeans")

    compressed = preprocessing.samples_to_image(
        result.compressed_samples, image.height, image.width
    )
    stopwatch.tick("reconstruct")

    image_io.ensure_directory(options.output_dir)
    output_path = image_io.make_output_path(
        options.input_path, options.output_dir, options.clusters, options.jpeg_quality
    )
    output_image = image_io.array_to_image(compressed)
    image_io.save_jpeg(output_path, output_image, options.jpeg_quality)
    stopwatch.tick("write")

    console.info(f"[OK] Saved: {output_path}")
    timings = " ".join(f"{event.label}={event.milliseconds}ms" for event in stopwatch.events)
    console.info(f"[TIMING] {timings} total={stopwatch.total_ms}ms")

    return output_path

