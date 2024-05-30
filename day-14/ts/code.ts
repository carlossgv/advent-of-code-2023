class Base {
  spaces: Buffer;
  width: number;
  height: number;

  constructor(base: Buffer) {
    this.spaces = base;
    const { width, height } = this.getDimensions(base);
    this.width = width;
    this.height = height;
  }

  calcNorthLoad = (): number => {
    let result = 0;
    for (let x = 0; x < this.width; x++) {
      for (let y = 0; y < this.height; y++) {
        const index = x + x * this.width + y;
        const current = this.spaces[index];

        if (current === 79) {
          result = result + this.height - x;
        }
      }
    }
    return result;
  };

  getDiagram = (): string => {
    let result = "\n";
    for (let i = 0; i < this.spaces.length; i++) {
      result = result + String.fromCharCode(this.spaces[i]);
    }
    return result;
  };

  private getDimensions = (base: Buffer) => {
    const width = base.indexOf(10); // ASCII code for newline '\n' is 10

    if (width === -1) {
      return { width: base.length, height: 1 };
    }

    let height = 0;
    for (let i = 0; i < base.length; i++) {
      if (base[i] === 10) {
        height++;
      }
    }

    if (base[base.length - 1] !== 10) {
      height++;
    }

    return { width, height };
  };
}

const cycle = (base: Base): void => {
  const directions = ["N", "W", "S", "E"];

  directions.forEach((direction) => {
    tilt(base, direction);
  });
};

const tilt = (base: Base, direction: string): Base => {
  if (direction === "N") {
    northTilt(base);
  }

  if (direction === "S") {
    southTilt(base);
  }

  if (direction === "E") {
    eastTilt(base);
  }

  if (direction === "W") {
    westTilt(base);
  }

  return base;
};

const northTilt = (base: Base) => {
  for (let y = 0; y < base.height; y++) {
    let barrierIndex = -(base.width + 1) + y;
    for (let x = 0; x < base.width; x++) {
      const index = x + x * base.width + y;
      const current = base.spaces[index];

      if (current === 35) {
        barrierIndex = index;
        continue;
      }

      if (current === 79) {
        base.spaces[index] = 46;
        barrierIndex = barrierIndex + (base.width + 1);
        base.spaces[barrierIndex] = 79;
      }
    }
  }
};

const westTilt = (base: Base) => {
  for (let y = 0; y < base.height; y++) {
    let barrierIndex = -1;
    for (let x = 0; x < base.width; x++) {
      const index = y * base.width + y + x;
      if (x === 0) {
        barrierIndex = index - 1;
      }
      const current = base.spaces[index];

      if (current === 35) {
        barrierIndex = index;
        continue;
      }

      if (current === 79) {
        base.spaces[index] = 46;
        barrierIndex = barrierIndex + 1;
        base.spaces[barrierIndex] = 79;
      }
    }
  }
};

const southTilt = (base: Base) => {
  for (let y = 0; y <= base.height; y++) {
    let barrierIndex = base.width + 1 + y;
    for (let x = base.width - 1; x >= 0; x--) {
      const index = x + x * base.width + y;
      if (x === base.width - 1) {
        barrierIndex = index + base.width + 1;
      }

      const current = base.spaces[index];

      if (current === 35) {
        barrierIndex = index;
        continue;
      }

      if (current === 79) {
        base.spaces[index] = 46;
        barrierIndex = barrierIndex - (base.width + 1);
        base.spaces[barrierIndex] = 79;
      }
    }
  }
};

const eastTilt = (base: Base) => {
  for (let y = 0; y < base.height; y++) {
    let barrierIndex = base.width;
    for (let x = base.width - 1; x >= 0; x--) {
      const index = y * base.width + y + x;
      if (x === base.width - 1) {
        barrierIndex = index + 1;
      }

      const current = base.spaces[index];

      if (current === 35) {
        barrierIndex = index;
        continue;
      }

      if (current === 79) {
        base.spaces[index] = 46;
        barrierIndex = barrierIndex - 1;
        base.spaces[barrierIndex] = 79;
      }
    }
  }
};

export { Base, cycle, tilt };
