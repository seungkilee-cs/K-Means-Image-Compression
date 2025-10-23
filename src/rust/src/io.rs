use anyhow::{Context, Result};
use image::codecs::jpeg::JpegEncoder;
use image::{ColorType, RgbImage};
use std::fs::File;
use std::io::BufWriter;
use std::path::{Path, PathBuf};

pub fn load_color_image<P: AsRef<Path>>(path: P) -> Result<RgbImage> {
    let image = image::open(&path).context("failed to open image")?.to_rgb8();
    Ok(image)
}

pub fn ensure_directory<P: AsRef<Path>>(dir: P) -> Result<()> {
    std::fs::create_dir_all(&dir).context("failed to create output directory")?;
    Ok(())
}

pub fn make_output_path<I, O>(input: I, output_dir: O, clusters: u32, quality: u8) -> PathBuf
where
    I: AsRef<Path>,
    O: AsRef<Path>,
{
    let stem = input
        .as_ref()
        .file_stem()
        .and_then(|s| s.to_str())
        .unwrap_or("image");
    let filename = format!("{}_k{}_q{}.jpg", stem, clusters, quality);
    output_dir.as_ref().join(filename)
}

pub fn save_jpeg<P: AsRef<Path>>(path: P, image: &RgbImage, quality: u8) -> Result<()> {
    let file = File::create(&path).context("failed to create output file")?;
    let mut writer = BufWriter::new(file);
    let mut encoder = JpegEncoder::new_with_quality(&mut writer, quality);
    encoder
        .encode(image.as_raw(), image.width(), image.height(), ColorType::Rgb8)
        .context("failed to encode JPEG")?;
    Ok(())
}
