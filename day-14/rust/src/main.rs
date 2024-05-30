use std::str;

struct Base {
    spaces: Vec<u8>,
    width: usize,
    height: usize,
}

impl Base {
    fn new(base: Vec<u8>) -> Self {
        let (width, height) = Self::get_dimensions(&base);
        Self {
            spaces: base,
            width,
            height,
        }
    }

    fn calc_north_load(&self) -> u32 {
        let mut result = 0;
        for x in 0..self.width {
            for y in 0..self.height {
                let index = x + x * self.width + y;
                let current = self.spaces[index];

                if current == b'O' {
                    result += (self.height - x) as u32;
                }
            }
        }
        result
    }

    // fn get_diagram(&self) -> String {
    //     let result: String = self.spaces.iter().map(|&c| c as char).collect();
    //     result
    // }

    fn get_dimensions(base: &Vec<u8>) -> (usize, usize) {
        let width = base.iter().position(|&c| c == b'\n').unwrap();
        let height = base.len() / (width + 1);
        (width, height)
    }
}

fn cycle(base: &mut Base) {
    let directions = ["N", "W", "S", "E"];

    for direction in &directions {
        tilt(base, direction);
    }
}

fn tilt(base: &mut Base, direction: &str) {
    match direction {
        "N" => north_tilt(base),
        "S" => south_tilt(base),
        "E" => east_tilt(base),
        "W" => west_tilt(base),
        _ => (),
    }
}

use rayon::prelude::*;

fn north_tilt(base: &mut Base) {
    let mut new_spaces = vec![0; base.spaces.len()];
    (0..base.height).into_par_iter().for_each(|y| {
        let mut barrier_index = -(base.width as isize + 1) + y as isize;
        let mut new_row = vec![0; base.width];
        for x in 0..base.width {
            // Remainder of your code remains the same
            let index = x + x * base.width + y;
            let current = base.spaces[index];

            if current == b'#' {
                barrier_index = index as isize;
                continue;
            }

            if current == b'O' {
                new_row[index] = b'.';
                barrier_index += base.width as isize + 1;
                new_row[barrier_index as usize] = b'O';
            }
        }

        new_spaces.push(new_row.iter().unwrap());
    });
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

            if current == b'#' {
                barrier_index = index as isize;
                continue;
            }

            if current == b'O' {
                base.spaces[index] = b'.';
                barrier_index += 1;
                base.spaces[barrier_index as usize] = b'O';
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

            if current == b'#' {
                barrier_index = index as isize;
                continue;
            }

            if current == b'O' {
                base.spaces[index] = b'.';
                barrier_index -= base.width as isize + 1;
                base.spaces[barrier_index as usize] = b'O';
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

            if current == b'#' {
                barrier_index = index as isize;
                continue;
            }

            if current == b'O' {
                base.spaces[index] = b'.';
                barrier_index -= 1;
                base.spaces[barrier_index as usize] = b'O';
            }
        }
    }
}

fn main() {
    let input = include_bytes!("input.txt").to_vec(); // Load input from file
    let cycles = 100000;

    println!("Running {} cycles...", cycles);
    let start = std::time::Instant::now();

    let mut base = Base::new(input);
    for _ in 0..cycles {
        cycle(&mut base);
    }

    println!("North load: {}", base.calc_north_load());
    let duration = start.elapsed();
    println!("Time: {:?}", duration);
}
