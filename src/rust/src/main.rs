mod algorithms;
mod cli;
mod io;
mod pipeline;
mod preprocessing;
mod timing;

use anyhow::Result;

fn main() -> Result<()> {
    let options = cli::parse();
    pipeline::run(options)?;
    Ok(())
}
