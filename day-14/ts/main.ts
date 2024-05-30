import { Base, cycle } from "./code";
import { INPUT } from "./input";
import fs from "fs";

let file = fs.readFileSync("../input.txt", "utf-8");
const input = Buffer.from(file, "utf-8");
const runCycles = (input: Buffer, cycles: number) => {
  const base = new Base(input);

  for (let i = 0; i < cycles; i++) {
    cycle(base);
  }

  console.log("North Load: ", base.calcNorthLoad());
};

const cycles = 100000;
console.log(`Running ${cycles} cycles...`);
const start = Date.now();
runCycles(input, cycles);
const end = Date.now();
console.log(`Time: ${end - start}ms`);
