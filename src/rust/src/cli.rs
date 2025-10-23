use clap::Parser;

#[derive(Debug, Parser)]
#[command(name = "imgc", about = "K-Means image compressor")]
pub struct Options {
    /// Input image path
    pub input_path: String,
    /// Number of clusters (2-256)
    pub clusters: u32,
    /// Output directory
    pub output_dir: String,
    /// JPEG quality (0-100)
    #[arg(long = "jpeg-quality", short = 'q', default_value_t = 95)]
    pub jpeg_quality: u8,
}

pub fn parse() -> Options {
    Options::parse()
}
