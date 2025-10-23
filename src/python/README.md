# Python Image Compression Module

## Overview
- **CLI**: `src/python/cli.py`
- **Image I/O & Preprocessing**: `src/python/image_io.py`, `src/python/preprocessing.py`
- **Algorithms**: `src/python/algorithms/`
- **Pipeline**: `src/python/pipeline.py`
- **Timing**: `src/python/timing.py`

## Setup
```bash
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt  # if available
```

## Run
```bash
python -m src.python.pipeline <input.png> <k> <output_dir> [--jpeg-quality Q]
```

## Tests
Add tests under `tests/` and run with your preferred framework (e.g., `pytest`).
