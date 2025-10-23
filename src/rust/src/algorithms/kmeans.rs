use anyhow::{ensure, Result};
use ndarray::{Array2, ArrayView1};
use rand::seq::SliceRandom;
use rand::thread_rng;

#[derive(Debug)]
pub struct CompressionResult {
    pub compressed_samples: Array2<f32>,
    pub labels: Vec<usize>,
    pub centers: Array2<f32>,
    pub compactness: f64,
}

#[derive(Debug, Clone, Copy)]
pub struct Config {
    pub max_iter: usize,
    pub tolerance: f32,
}

impl Default for Config {
    fn default() -> Self {
        Self {
            max_iter: 100,
            tolerance: 1e-4,
        }
    }
}

#[derive(Debug)]
pub struct KMeans {
    config: Config,
}

impl KMeans {
    pub fn new(config: Config) -> Self {
        Self { config }
    }

    pub fn compress(&self, samples: &Array2<f32>, clusters: usize) -> Result<CompressionResult> {
        ensure!(samples.ncols() == 3, "expected 3 columns (RGB)");
        ensure!(clusters >= 2, "clusters must be >= 2");
        ensure!(samples.nrows() >= clusters, "sample count must be >= clusters");

        let mut rng = thread_rng();
        let mut centers = initialize_centers(samples, clusters, &mut rng);
        let mut labels = vec![0usize; samples.nrows()];

        for _ in 0..self.config.max_iter {
            assign_clusters(samples, &centers, &mut labels);
            let new_centers = update_centers(samples, &labels, clusters);

            if has_converged(&centers, &new_centers, self.config.tolerance) {
                centers = new_centers;
                break;
            }

            centers = new_centers;
        }

        assign_clusters(samples, &centers, &mut labels);
        let compressed = build_compressed_samples(&centers, &labels);
        let compactness = compute_compactness(samples, &compressed);

        Ok(CompressionResult {
            compressed_samples: compressed,
            labels,
            centers,
            compactness,
        })
    }
}

impl Default for KMeans {
    fn default() -> Self {
        Self::new(Config::default())
    }
}

fn initialize_centers(
    samples: &Array2<f32>,
    clusters: usize,
    rng: &mut rand::rngs::ThreadRng,
) -> Array2<f32> {
    let mut indices: Vec<_> = (0..samples.nrows()).collect();
    indices.shuffle(rng);
    let selected = &indices[..clusters];

    let mut centers = Array2::<f32>::zeros((clusters, samples.ncols()));
    for (center_idx, &sample_idx) in selected.iter().enumerate() {
        centers
            .row_mut(center_idx)
            .assign(&samples.row(sample_idx));
    }
    centers
}

fn assign_clusters(samples: &Array2<f32>, centers: &Array2<f32>, labels: &mut [usize]) {
    for (i, sample) in samples.outer_iter().enumerate() {
        let mut best_cluster = 0usize;
        let mut best_dist = f32::MAX;

        for (cluster_idx, center) in centers.outer_iter().enumerate() {
            let dist = squared_distance(&sample, &center);
            if dist < best_dist {
                best_dist = dist;
                best_cluster = cluster_idx;
            }
        }
        labels[i] = best_cluster;
    }
}

fn update_centers(samples: &Array2<f32>, labels: &[usize], clusters: usize) -> Array2<f32> {
    let mut centers = Array2::<f32>::zeros((clusters, samples.ncols()));
    let mut counts = vec![0usize; clusters];

    for (sample, &label) in samples.outer_iter().zip(labels.iter()) {
        let mut row = centers.row_mut(label);
        row += &sample;
        counts[label] += 1;
    }

    for (mut center, count) in centers.outer_iter_mut().zip(counts.iter()) {
        if *count > 0 {
            center.map_inplace(|x| *x /= *count as f32);
        }
    }

    centers
}

fn has_converged(old: &Array2<f32>, new: &Array2<f32>, tolerance: f32) -> bool {
    let mut max_shift = 0f32;
    for (old_center, new_center) in old.outer_iter().zip(new.outer_iter()) {
        let shift = squared_distance(&old_center, &new_center).sqrt();
        if shift > max_shift {
            max_shift = shift;
        }
    }
    max_shift <= tolerance
}

fn build_compressed_samples(centers: &Array2<f32>, labels: &[usize]) -> Array2<f32> {
    let mut output = Array2::<f32>::zeros((labels.len(), centers.ncols()));
    for (i, &label) in labels.iter().enumerate() {
        output.row_mut(i).assign(&centers.row(label));
    }
    output
}

fn compute_compactness(samples: &Array2<f32>, compressed: &Array2<f32>) -> f64 {
    samples
        .outer_iter()
        .zip(compressed.outer_iter())
        .map(|(orig, comp)| squared_distance(&orig, &comp) as f64)
        .sum()
}

fn squared_distance(a: &ArrayView1<f32>, b: &ArrayView1<f32>) -> f32 {
    a.iter()
        .zip(b.iter())
        .map(|(x, y)| (x - y).powi(2))
        .sum()
}
