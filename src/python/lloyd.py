# lloyd's algorithm implementation
import numpy as np


def _validate_input(data: np.ndarray, k: int) -> None:
    if data.ndim != 2:
        raise ValueError("data must be 2-dimensional")
    if data.shape[0] < k:
        raise ValueError("number of samples must be >= k")
    if k < 2:
        raise ValueError("k must be at least 2")


def initialize_centroids(data: np.ndarray, k: int) -> np.ndarray:
    """Randomly select k data points as initial centroids"""
    _validate_input(data, k)
    rng = np.random.default_rng()
    indices = rng.choice(data.shape[0], size=k, replace=False)
    return data[indices].astype(np.float32, copy=True)


def assign_clusters(data: np.ndarray, centroids: np.ndarray) -> np.ndarray:
    """Assign each data point to nearest centroids and return array of cluster indices"""
    distances = np.linalg.norm(data[:, None, :] - centroids[None, :, :], axis=2)
    return np.argmin(distances, axis=1).astype(np.int32, copy=False)


def update_centroids(data: np.ndarray, labels: np.ndarray, k: int) -> np.ndarray:
    """Update centroids as the mean of assigned data points"""
    n_features = data.shape[1]
    new_centroids = np.empty((k, n_features), dtype=np.float32)
    rng = np.random.default_rng()
    for cluster_id in range(k):
        mask = labels == cluster_id
        if not np.any(mask):
            new_centroids[cluster_id] = data[rng.integers(0, data.shape[0])]
        else:
            new_centroids[cluster_id] = data[mask].mean(axis=0, dtype=np.float64)
    return new_centroids.astype(np.float32, copy=False)


def has_converged(
    old_centroid: np.ndarray, new_centroid: np.ndarray, tolerance: float = 1e-4
) -> bool:
    """check if the cntroids have converged within the set tolerance"""
    shift = np.linalg.norm(new_centroid - old_centroid)
    return shift <= tolerance


def kmeans(
    data: np.ndarray, k: int, max_iter: int = 100, tolerance: float = 1e-4
) -> tuple[np.ndarray, np.ndarray]:
    """
    run lloyd's k means algorithm, returning tuple of centroids and labels
        centroids: np.ndarray of shape (k, features)
        labels: np.ndarray of shape (n_samples,)
    """
    _validate_input(data, k)

    centroids = initialize_centroids(data, k)
    labels = assign_clusters(data, centroids)

    for _ in range(max_iter):
        new_centroids = update_centroids(data, labels, k)
        if has_converged(centroids, new_centroids, tolerance):
            centroids = new_centroids
            break
        centroids = new_centroids
        labels = assign_clusters(data, centroids)
    else:
        labels = assign_clusters(data, centroids)

    return centroids.astype(np.float32, copy=False), labels.astype(np.int32, copy=False)
