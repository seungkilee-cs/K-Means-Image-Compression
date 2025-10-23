import { KMeans } from "./algorithms/kmeans.js";
import { Options } from "./cli.js";
import { ensureDirectory, loadColorImage, makeOutputPath, saveJpeg } from "./io.js";
import { imageToSamples, samplesToImage } from "./preprocessing.js";
import { Stopwatch } from "./timing.js";

export async function run(options: Options): Promise<string> {
  console.log(`[INFO] Input: ${options.inputPath}`);
  console.log(
    `[INFO] k=${options.clusters} out_dir=${options.outputDir} quality=${options.jpegQuality}`,
  );

  const stopwatch = new Stopwatch();

  const image = await loadColorImage(options.inputPath);
  console.log(`[INFO] Loaded ${image.width}x${image.height}`);
  stopwatch.tick("load");

  const samples = imageToSamples(image);
  console.log(`[INFO] Samples: ${samples.rows} x ${samples.cols}`);
  stopwatch.tick("prep");

  const kmeans = new KMeans();
  console.log("[INFO] Running kmeans...");
  const result = kmeans.compress(samples, options.clusters);
  console.log(`[INFO] Kmeans done. Compactness=${result.compactness.toFixed(2)}`);
  stopwatch.tick("kmeans");

  const compressedImage = samplesToImage(result.compressed, image.width, image.height);
  stopwatch.tick("reconstruct");

  await ensureDirectory(options.outputDir);
  const outputPath = makeOutputPath(
    options.inputPath,
    options.outputDir,
    options.clusters,
    options.jpegQuality,
  );
  await saveJpeg(outputPath, compressedImage, options.jpegQuality);
  stopwatch.tick("write");

  console.log(`[OK] Saved: ${outputPath}`);
  printTimings(stopwatch);

  return outputPath;
}

function printTimings(stopwatch: Stopwatch) {
  const parts = stopwatch
    .getEvents()
    .map((event) => `${event.label}=${event.milliseconds}ms`)
    .join(" ");
  console.log(`[TIMING] ${parts} total=${stopwatch.totalMs()}ms`);
}
