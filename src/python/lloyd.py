# lloyd's algorithm implementation
import numpy as np


def initialize_centroids(data: np.ndarray, k: int) -> np.ndarray:
    """Randomly select k data points as initial centroids"""
    pass


def assign_clusters(data: np.ndarray, centroids: np.ndarray) -> np.ndarray:
    """Assign each data point to nearest centroids and return array of cluster indices"""
    pass


def update_centroids(data: np.ndarray, labels: np.ndarray, k: int) -> np.ndarray:
    """Update centroids as the mean of assigned data points"""
    pass


def has_converged(
    old_centroid: np.ndarray, new_centroid: np.ndarray, tolerance: float = 1e-4
) -> bool:
    """check if the cntroids have converged within the set tolerance"""
    return True


def kmeans(
    data: np.ndarray, k: int, max_iter: int = 100, tolerance: float = 1e-4
) -> tuple[np.ndarray, np.ndarray]:
    """
    run lloyd's k means algorithm, returning tuple of centroids and labels
        centroids: np.ndarray of shape (k, features)
        labels: np.ndarray of shape (n_samples,)
    """
    return (None, None)
