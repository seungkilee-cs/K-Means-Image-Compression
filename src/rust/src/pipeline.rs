use crate::algorithms::KMeans;
use crate::cli::Options;
use crate::io;
use crate::preprocessing;
use crate::timing::Stopwatch;
use anyhow::{Context, Result};

pub fn run(options: Options) -> Result<()> {
    println!(
        "[INFO] Input: {}\n[INFO] k={} out_dir={} quality={}",
        options.input_path, options.clusters, options.output_dir, options.jpeg_quality
    );

    let mut stopwatch = Stopwatch::new();

    let image = io::load_color_image(&options.input_path).context("load image")?;
    let width = image.width();
    let height = image.height();
    println!("[INFO] Loaded {}x{}", width, height);
    stopwatch.tick("load");

    let samples = preprocessing::image_to_samples(&image).context("prepare samples")?;
    println!(
        "[INFO] Samples: {} x {}",
        samples.nrows(),
        samples.ncols()
    );
    stopwatch.tick("prep");

    let kmeans = KMeans::default();
    println!("[INFO] Running kmeans...");
    let result = kmeans
        .compress(&samples, options.clusters as usize)
        .context("kmeans")?;
    println!("[INFO] Kmeans done. Compactness={:.2}", result.compactness);
    stopwatch.tick("kmeans");

    let compressed_image = preprocessing::samples_to_image(
        &result.compressed_samples,
        width,
        height,
    )
    .context("reconstruct image")?;
    stopwatch.tick("reconstruct");

    io::ensure_directory(&options.output_dir).context("ensure output directory")?;
    let output_path = io::make_output_path(
        &options.input_path,
        &options.output_dir,
        options.clusters,
        options.jpeg_quality,
    );
    io::save_jpeg(&output_path, &compressed_image, options.jpeg_quality)
        .context("save jpeg")?;
    stopwatch.tick("write");

    println!("[OK] Saved: {}", output_path.display());
    print_timings(&stopwatch);

    Ok(())
}

fn print_timings(stopwatch: &Stopwatch) {
    let mut parts = Vec::new();
    for event in stopwatch.events() {
        parts.push(format!("{}={}ms", event.label, event.milliseconds));
    }
    println!("[TIMING] {} total={}ms", parts.join(" "), stopwatch.total_ms());
}
