# driver for image processing
import numpy as np
from PIL import Image


def load_image(path: str) -> Image.Image:
    """Load image and return as numpy array"""
    pass


def save_image(image: Image.Image, path: str) -> None:
    """Save PIL Image to file path"""
    pass


def image_to_array(image: Image.Image) -> np.ndarray:
    """Convert PIL Image to np array"""
    pass


def array_to_image(arr: np.ndarray) -> Image.Image:
    """Convert np array to PIL Image"""
    pass


def flatten_image_array(arr: np.ndarray) -> np.ndarray:
    """flatten 3d image array of width, height and color to 2d array of N_pixels, Color"""
    pass


def unflatten_image_array(arr: np.ndarray) -> np.ndarray:
    """unflatten 2d array of N_pixels, Color to width, height and color"""
    pass
