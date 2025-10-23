import { promises as fs } from "node:fs";
import { dirname, join, parse } from "node:path";
import sharp from "sharp";

export interface ImageData {
  data: Uint8Array;
  width: number;
  height: number;
}

export async function loadColorImage(path: string): Promise<ImageData> {
  const { data, info } = await sharp(path)
    .removeAlpha()
    .raw()
    .toBuffer({ resolveWithObject: true });

  if (info.channels !== 3) {
    throw new Error("expected RGB channels");
  }

  return { data: new Uint8Array(data), width: info.width, height: info.height };
}

export async function ensureDirectory(path: string): Promise<void> {
  await fs.mkdir(path, { recursive: true });
}

export function makeOutputPath(
  inputPath: string,
  outputDir: string,
  clusters: number,
  quality: number,
): string {
  const parsed = parse(inputPath);
  const filename = `${parsed.name}_k${clusters}_q${quality}.jpg`;
  return join(outputDir, filename);
}

export async function saveJpeg(
  path: string,
  image: ImageData,
  quality: number,
): Promise<void> {
  await ensureDirectory(dirname(path));

  await sharp(image.data, {
    raw: {
      width: image.width,
      height: image.height,
      channels: 3,
    },
  })
    .jpeg({ quality })
    .toFile(path);
}
