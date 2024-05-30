import { Base, cycle, tilt } from "./code";
// import { getArrayFromInput, tiltNorth, tilt, cycle } from "./main-alex";
// import { INPUT } from "./input";
// import TEST_INPUT from "./test-input";

const flatBase = `O....#....
O.OO#....#
.....##...
OO.#O....O
.O.....O#.
O.#..O.#.#
..O..#O..O
.......O..
#....###..
#OO..#....`;

//   0123456789
//  y x-------->
// 0| O....#....
// 1| O.OO#....#
// 2| .....##...
// 3| OO.#O....O
// 4| .O.....O#.
// 5| O.#..O.#.#
// 6| ..O..#O..O
// 7| .......O..
// 8| #....###..
// 9v #OO..#....`
const tiltedWest = `O....#....
OOO.#....#
.....##...
OO.#OO....
OO......#.
O.#O...#.#
O....#OO..
O.........
#....###..
#OO..#....`;

const tiltedEast = `....O#....
.OOO#....#
.....##...
.OO#....OO
......OO#.
.O#...O#.#
....O#..OO
.........O
#....###..
#..OO#....`;

const tiltedNorth = `
OOOO.#.O..
OO..#....#
OO..O##..O
O..#.OO...
........#.
..#....#.#
..O..#.O.O
..O.......
#....###..
#....#....`;

const tiltedSouth = `.....#....
....#....#
...O.##...
...#......
O.O....O#O
O.#..O.#.#
O....#....
OO....OO..
#OO..###..
#OO.O#...O`;

// After 1 cycle:
// .....#....
// ....#...O#
// ...OO##...
// .OO#......
// .....OOO#.
// .O#...O#.#
// ....O#....
// ......OOOO
// #...O###..
// #..OO#....
//
// After 2 cycles:
// .....#....
// ....#...O#
// .....##...
// ..O#......
// .....OOO#.
// .O#...O#.#
// ....O#...O
// .......OOO
// #..OO###..
// #.OOO#...O
//
// After 3 cycles:
// .....#....
// ....#...O#
// .....##...
// ..O#......
// .....OOO#.
// .O#...O#.#
// ....O#...O
// .......OOO
// #...O###.O
// #.OOO#...O

const expectedFirstCycle = `
.....#....
....#...O#
...OO##...
.OO#......
.....OOO#.
.O#...O#.#
....O#....
......OOOO
#...O###..
#..OO#....
`;

const expectedSecondCycle = `
.....#....
....#...O#
.....##...
..O#......
.....OOO#.
.O#...O#.#
....O#...O
.......OOO
#..OO###..
#.OOO#...O
`;

const expectedThirdCycle = `
.....#....
....#...O#
.....##...
..O#......
.....OOO#.
.O#...O#.#
....O#...O
.......OOO
#...O###.O
#.OOO#...O
`;

const testNorthTilt = (base: Buffer, expected: string) => {
  const baseObj = new Base(base);
  tilt(baseObj, "N");
  const actual = baseObj.getDiagram();
  if (actual.trim() !== expected.trim()) {
    console.error("Expected:", expected);
    console.error("\nFailed:", actual);
  } else {
    console.log("nort tilt Passed");
    console.log("load", baseObj.calcNorthLoad());
  }
};

const testSouthTilt = (base: Buffer, expected: string) => {
  const baseObj = new Base(base);
  tilt(baseObj, "S");
  const actual = baseObj.getDiagram();
  if (actual.trim() !== expected.trim()) {
    console.error("Expected:", expected);
    console.error("\nFailed:", actual);
  } else {
    console.log("south tilt Passed");
    console.log("load", baseObj.calcNorthLoad());
  }
};

const testEastTilt = (base: Buffer, expected: string) => {
  const baseObj = new Base(base);
  tilt(baseObj, "E");
  const actual = baseObj.getDiagram();
  if (actual.trim() !== expected.trim()) {
    console.error("Expected:", expected);
    console.error("Failed:", actual);
  } else {
    console.log("east tilt Passed");
    console.log("load", baseObj.calcNorthLoad());
  }
};

const testWestTilt = (base: Buffer, expected: string) => {
  const baseObj = new Base(base);
  tilt(baseObj, "W");
  const actual = baseObj.getDiagram();
  if (actual.trim() !== expected.trim()) {
    console.error("Expected:", expected);
    console.error("Failed:", actual);
  } else {
    console.log("west tilt Passed");
    console.log("load", baseObj.calcNorthLoad());
  }
};

const testLoad = (base: Buffer, expected: number) => {
  const baseObj = new Base(base);
  tilt(baseObj, "N");
  const actual = baseObj.calcNorthLoad();
  console.log('diagr', baseObj.getDiagram())
  if (actual !== expected) {
    console.error("Failed:", actual);
  } else {
    console.log("load Passed");
  }
};

const testCycles = (baseString: Buffer, cycles: number) => {
  const base = new Base(baseString);

  for (let i = 0; i < cycles; i++) {
    cycle(base);
    let expectedDiagram: string = "";
    if (i === 0) {
      expectedDiagram = expectedFirstCycle;
    }

    if (i === 1) {
      expectedDiagram = expectedSecondCycle;
    }

    if (i === 2) {
      expectedDiagram = expectedThirdCycle;
    }

    const actual = base.getDiagram();
    if (actual !== expectedDiagram) {
      console.error("Cycle:", i + 1);
      console.error("Expected:", expectedDiagram);
      console.error("Failed:", actual);
    } else {
      console.log(`${i + 1} cycle Passed`);
      console.log("load", base.calcNorthLoad());
    }
  }
};
//
// console.log("Running tests...");
// // testCycles(flatBase, 1000000000, 64);
//
// read file test-input.txt to get buffer
const input = Buffer.from(flatBase, "utf-8");

// testNorthTilt(input, tiltedNorth);
// testSouthTilt(input, tiltedSouth);
// testEastTilt(input, tiltedEast);
// testWestTilt(input, tiltedWest);
testLoad(input, 136);
// console.debug("base", flatBase, "\n");
// testCycles(input, 3);
//
