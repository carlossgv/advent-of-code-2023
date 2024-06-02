const hashChar = (char: string, hash: number = 0): number => {
  hash += char.charCodeAt(0);
  hash *= 17;
  hash %= 256;
  return hash;
};

const hashString = (str: string, hash = 0): number => {
  for (let i = 0; i < str.length; i++) {
    hash = hashChar(str[i], hash);
  }
  return hash;
};

const hashArray = (input: string): number => {
  const strings = input.split(",");
  const results: number[] = [];
  for (let i = 0; i < strings.length; i++) {
    results.push(hashString(strings[i]));
  }
  return results.reduce((acc, val) => acc + val, 0);
};

const calcFocusingPower = (input: string): number => {
  const labels = input.split(",");
  const hashMap: { [key: string]: Label[] } = {};

  for (let i = 0; i < labels.length; i++) {
    const parsedLabel = parseLabel(labels[i]);
    assignLens(parsedLabel, hashMap);
  }

  const boxes = Object.keys(hashMap);
  let focusingPower = 0;
  for (let i = 0; i < boxes.length; i++) {
    const labels = hashMap[boxes[i]];
    if (!labels.length) {
      continue;
    }
    for (let j = 0; j < labels.length; j++) {
      const boxValue = parseInt(boxes[i]) + 1;
      const indexValue = j + 1;
      if (!labels[j].focalLength) {
        throw new Error("Focal length not set");
      }
      const labelPower = boxValue * indexValue * labels[j].focalLength;
      focusingPower += labelPower;
    }
  }

  return focusingPower;
};

const assignLens = (
  label: Label,
  hashMap: { [key: string]: Label[] },
): void => {
  if (label.operand === "-") {
    if (!hashMap[label.box]) {
      return;
    }
    hashMap[label.box] = hashMap[label.box].filter(
      (l) => l.label !== label.label,
    );
  } else if (label.operand === "=") {
    const labels = hashMap[label.box];
    if (!labels || !labels.length) {
      hashMap[label.box] = [label];
      return;
    }
    const existingLabel = labels.find((l) => l.label === label.label);
    if (existingLabel) {
      existingLabel.focalLength = label.focalLength;
    }

    if (!existingLabel) {
      labels.push(label);
    }
  } else {
    throw new Error("Invalid operand");
  }
};

type Label = {
  label: string;
  box: number;
  operand: "=" | "-";
  focalLength: number;
};

const parseLabel = (completeLabel: string): Label => {
  const operand = completeLabel.includes("=") ? "=" : "-";
  let [label, focalLength] = completeLabel.split(operand);
  const box = hashString(label);
  return { label, operand, box, focalLength: parseInt(focalLength) || 0 };
};

export { hashChar, hashString, hashArray, parseLabel, calcFocusingPower };
