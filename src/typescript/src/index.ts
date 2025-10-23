import { run } from "./pipeline.js";
import { parse } from "./cli.js";

async function main() {
  try {
    const options = parse(process.argv);
    await run(options);
  } catch (err) {
    console.error("[ERROR]", err instanceof Error ? err.message : err);
    process.exit(1);
  }
}

void main();
