use anyhow::{ensure, Result};
use image::RgbImage;
use ndarray::Array2;

pub fn image_to_samples(image: &RgbImage) -> Result<Array2<f32>> {
    let (width, height) = (image.width() as usize, image.height() as usize);
    ensure!(width > 0 && height > 0, "image dimensions must be positive");

    let mut samples = Array2::<f32>::zeros((width * height, 3));
    for (idx, pixel) in image.pixels().enumerate() {
        samples[[idx, 0]] = pixel[0] as f32;
        samples[[idx, 1]] = pixel[1] as f32;
        samples[[idx, 2]] = pixel[2] as f32;
    }
    Ok(samples)
}

pub fn samples_to_image(samples: &Array2<f32>, width: u32, height: u32) -> Result<RgbImage> {
    ensure!(samples.ncols() == 3, "samples must have 3 columns");
    ensure!(samples.nrows() == (width * height) as usize, "row count mismatch");

    let mut image = RgbImage::new(width, height);
    for (idx, pixel) in image.pixels_mut().enumerate() {
        let row = samples.row(idx);
        pixel[0] = row[0].clamp(0.0, 255.0) as u8;
        pixel[1] = row[1].clamp(0.0, 255.0) as u8;
        pixel[2] = row[2].clamp(0.0, 255.0) as u8;
    }
    Ok(image)
}
