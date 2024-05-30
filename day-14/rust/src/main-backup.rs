mod input;
type Spaces = Vec<Vec<char>>;

struct Base {
    spaces: Spaces,
    width: usize,
    height: usize,
}

impl Base {
    fn new(base: &str) -> Self {
        let spaces = Self::parse_string_to_space(base);
        let (width, height) = Self::get_dimensions(&spaces);
        Base {
            spaces,
            width,
            height,
        }
    }

    fn calc_north_load(&self) -> usize {
        let mut result = 0;
        for x in 0..=self.width {
            for y in 0..=self.height {
                let current = self.spaces[x][y];
                if current == 'O' {
                    result += self.height + 1 - x;
                }
            }
        }
        result
    }

    // fn get_diagram(&self) -> String {
    //     let mut result = String::from("\n");
    //     for x in 0..=self.width {
    //         for y in 0..=self.height {
    //             result.push(self.spaces[x][y]);
    //         }
    //         result.push('\n');
    //     }
    //     result
    // }

    fn parse_string_to_space(base: &str) -> Spaces {
        base.trim()
            .lines()
            .map(|line| line.chars().collect())
            .collect()
    }

    fn get_dimensions(spaces: &Spaces) -> (usize, usize) {
        let width = spaces[0].len() - 1;
        let height = spaces.len() - 1;
        (width, height)
    }
}

fn cycle(base: &mut Base) {
    let directions = ["N", "W", "S", "E"];

    for direction in directions.iter() {
        tilt(base, direction);
    }

    base.calc_north_load();
}

fn tilt(base: &mut Base, direction: &str) {
    match direction {
        "N" => north_tilt(base),
        "S" => south_tilt(base),
        "E" => east_tilt(base),
        "W" => west_tilt(base),
        _ => {}
    }
}

fn north_tilt(base: &mut Base) {
    for y in 0..=base.height {
        // TODO: run this in parallel
        let mut barrier = (-1, y);
        for x in 0..=base.width {
            if base.spaces[x][y] == '#' {
                barrier = (x as isize, y);
                continue;
            }

            if base.spaces[x][y] == 'O' {
                base.spaces[x][y] = '.';
                base.spaces[(barrier.0 + 1) as usize][y] = 'O';
                barrier = (barrier.0 + 1, y);
            }
        }
    }
}

fn south_tilt(base: &mut Base) {
    for y in 0..=base.height {
        // TODO: run this in parallel
        let mut barrier = (base.width as isize + 1, y);
        for x in (0..=base.width).rev() {
            if base.spaces[x][y] == '#' {
                barrier = (x as isize, y);
                continue;
            }

            if base.spaces[x][y] == 'O' {
                base.spaces[x][y] = '.';
                base.spaces[(barrier.0 - 1) as usize][y] = 'O';
                barrier = (barrier.0 - 1, y);
            }
        }
    }
}

fn west_tilt(base: &mut Base) {
    for x in 0..=base.width {
        // TODO: run this in parallel
        let mut barrier = (x, -1);
        for y in 0..=base.height {
            if base.spaces[x][y] == '#' {
                barrier = (x, y as isize);
                continue;
            }

            if base.spaces[x][y] == 'O' {
                base.spaces[x][y] = '.';
                base.spaces[x][(barrier.1 + 1) as usize] = 'O';
                barrier = (x, barrier.1 + 1);
            }
        }
    }
}

fn east_tilt(base: &mut Base) {
    for x in 0..=base.width {
        // TODO: run this in parallel
        let mut barrier = (x, base.height as isize + 1);
        for y in (0..=base.height).rev() {
            if base.spaces[x][y] == '#' {
                barrier = (x, y as isize);
                continue;
            }

            if base.spaces[x][y] == 'O' {
                base.spaces[x][y] = '.';
                base.spaces[x][(barrier.1 - 1) as usize] = 'O';
                barrier = (x, barrier.1 - 1);
            }
        }
    }
}

fn main() {
    const CYCLES: usize = 100000;
    println!("Running {} cycles ...", CYCLES);

    let mut base = Base::new(input::INPUT);
    let start = std::time::Instant::now();

    for _ in 0..CYCLES {
        cycle(&mut base);
    }

    let elapsed = start.elapsed();

    println!("Load: {}", base.calc_north_load());
    println!("Time: {:?}", elapsed);
}
