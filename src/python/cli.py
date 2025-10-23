from __future__ import annotations

import argparse
from dataclasses import dataclass


@dataclass(frozen=True)
class Options:
    input_path: str
    clusters: int
    output_dir: str
    jpeg_quality: int = 95


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        prog="kmeans_imgc",
        description="Apply k-means color quantization to an image.",
    )
    parser.add_argument("input", help="input image path")
    parser.add_argument("k", type=int, help="number of clusters (2-256)")
    parser.add_argument("output_dir", help="directory where the JPEG will be written")
    parser.add_argument(
        "--jpeg-quality",
        type=int,
        default=95,
        metavar="Q",
        help="JPEG quality 0-100 (default: 95)",
    )
    return parser


def parse_args(argv: list[str] | None = None) -> Options:
    parser = build_parser()
    args = parser.parse_args(argv)

    if not 2 <= args.k <= 256:
        parser.error("k must be in [2, 256]")
    if not 0 <= args.jpeg_quality <= 100:
        parser.error("jpeg_quality must be in [0, 100]")

    return Options(
        input_path=args.input,
        clusters=args.k,
        output_dir=args.output_dir,
        jpeg_quality=args.jpeg_quality,
    )

