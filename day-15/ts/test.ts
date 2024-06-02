import fs from "fs";
import { calcFocusingPower, hashArray, hashString, parseLabel } from "./code";
let input = fs.readFileSync("../test-input.txt", "utf8");
input = input.slice(0, -1);

const testHash = (string: string, expected: number) => {
  const hash = hashString(string);
  if (hash !== expected) {
    console.error(`Test failed: expected ${expected}, got ${hash}`);
  } else {
    console.log(`Test passed: ${string} -> ${hash}`);
  }
};

const testInputHash = (input: string, expected: number) => {
  const hash = hashArray(input);
  if (hash !== expected) {
    console.error(`Test failed: expected ${expected}, got ${hash}`);
  } else {
    console.log(`Test passed: ${input} -> ${hash}`);
  }
};

const testFocusPower = (input: string, expected: number) => {
  const power = calcFocusingPower(input);
  if (power !== expected) {
    console.error(`Test failed: expected ${expected}, got ${power}`);
  } else {
    console.log(`Test passed: ${input} -> ${power}`);
  }
};

// testHash("HASH", 52);
// testInputHash(input, 1320);
testFocusPower(input, 145);
