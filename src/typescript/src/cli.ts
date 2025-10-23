import { Command } from "commander";

export interface Options {
  inputPath: string;
  clusters: number;
  outputDir: string;
  jpegQuality: number;
}

export function parse(argv: string[]): Options {
  const program = new Command();

  program
    .name("kmeans-imgc")
    .description("Apply k-means color quantization to an image")
    .argument("<input>", "input image path")
    .argument("<k>", "number of clusters (2-256)")
    .argument("<outputDir>", "directory where the JPEG will be written")
    .option("-q, --jpeg-quality <quality>", "JPEG quality 0-100", "95")
    .parse(argv);

  const opts = program.opts();
  const [inputPath, kRaw, outputDir] = program.args as [string, string, string];

  const clusters = parseInt(kRaw, 10);
  const jpegQuality = parseInt(opts.jpegQuality, 10);

  if (!Number.isFinite(clusters) || clusters < 2 || clusters > 256) {
    throw new Error("k must be in [2, 256]");
  }

  if (!Number.isFinite(jpegQuality) || jpegQuality < 0 || jpegQuality > 100) {
    throw new Error("jpeg_quality must be in [0, 100]");
  }

  return {
    inputPath,
    clusters,
    outputDir,
    jpegQuality,
  };
}
