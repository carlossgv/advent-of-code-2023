import fs from "fs";
import { calcFocusingPower } from "./code";
let input = fs.readFileSync("../input.txt", "utf8");
input = input.slice(0, -1);

const start = Date.now();
const response = calcFocusingPower(input);
console.log(`Focusing power:`, response);
console.log(`Time taken: ${Date.now() - start}ms`);
