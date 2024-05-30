use std::fs::File;
use std::io::{self, Read};
use std::str;

const INPUT_FILE: &str = "../input.txt";

// ASCII codes
const DOT: u8 = 46;
const HASH: u8 = 35;
const O: u8 = 79;
const NEWLINE: u8 = 10;

struct Base {
    spaces: Vec<u8>,
    width: usize,
    height: usize,
}

fn get_dimensions(base: &[u8]) -> (usize, usize) {
    let width = base.iter().position(|&x| x == NEWLINE).unwrap();
    let height = base.len() / (width + 1);
    (width, height)
}

fn calc_north_load(base: &Base) -> usize {
    let mut result = 0;
    for x in 0..base.width {
        for y in 0..base.height {
            let index = x + x * base.width + y;
            let current = base.spaces[index];
            if current == O {
                result += base.height - x;
            }
        }
    }
    result
}

fn north_tilt(base: &mut Base) {
    for y in 0..base.height {
        let mut barrier_index = -(base.width as isize + 1) + y as isize;
        for x in 0..base.width {
            let index = x + x * base.width + y;
            let current = base.spaces[index];
            if current == HASH {
                barrier_index = index as isize;
                continue;
            }
            if current == O {
                base.spaces[index] = DOT;
                barrier_index += base.width as isize + 1;
                base.spaces[barrier_index as usize] = O;
            }
        }
    }
}

fn west_tilt(base: &mut Base) {
    for y in 0..base.height {
        let mut barrier_index = -1;
        for x in 0..base.width {
            let index = y * base.width + y + x;
            if x == 0 {
                barrier_index = index as isize - 1;
            }
            let current = base.spaces[index];
            if current == HASH {
                barrier_index = index as isize;
                continue;
            }
            if current == O {
                base.spaces[index] = DOT;
                barrier_index += 1;
                base.spaces[barrier_index as usize] = O;
            }
        }
    }
}

fn south_tilt(base: &mut Base) {
    for y in 0..=base.height {
        let mut barrier_index = base.width as isize + 1 + y as isize;
        for x in (0..base.width).rev() {
            let index = x + x * base.width + y;
            if x == base.width - 1 {
                barrier_index = index as isize + base.width as isize + 1;
            }
            let current = base.spaces[index];
            if current == HASH {
                barrier_index = index as isize;
                continue;
            }
            if current == O {
                base.spaces[index] = DOT;
                barrier_index -= base.width as isize + 1;
                base.spaces[barrier_index as usize] = O;
            }
        }
    }
}

fn east_tilt(base: &mut Base) {
    for y in 0..base.height {
        let mut barrier_index = base.width as isize;
        for x in (0..base.width).rev() {
            let index = y * base.width + y + x;
            if x == base.width - 1 {
                barrier_index = index as isize + 1;
            }
            let current = base.spaces[index];
            if current == HASH {
                barrier_index = index as isize;
                continue;
            }
            if current == O {
                base.spaces[index] = DOT;
                barrier_index -= 1;
                base.spaces[barrier_index as usize] = O;
            }
        }
    }
}

fn cycle(base: &mut Base) {
    north_tilt(base);
    west_tilt(base);
    south_tilt(base);
    east_tilt(base);
}

fn run_cycles(input: &[u8], cycles: usize) {
    let (width, height) = get_dimensions(input);
    let mut base = Base {
        spaces: input.to_vec(),
        width,
        height,
    };

    for _ in 0..cycles {
        cycle(&mut base);
    }

    println!("North Load: {}", calc_north_load(&base));
}

fn main() -> io::Result<()> {
    let mut file = File::open(INPUT_FILE)?;
    // show error if file is not found
    if file.metadata().is_err() {
        return Err(io::Error::new(io::ErrorKind::NotFound, "File not found"));
    }
    let mut input = Vec::new();
    file.read_to_end(&mut input)?;

    let cycles = 100000;
    println!("Running {} cycles...", cycles);

    let start = std::time::Instant::now();
    run_cycles(&input, cycles);
    let duration = start.elapsed();

    println!("Time: {}ms", duration.as_millis());

    Ok(())
}
