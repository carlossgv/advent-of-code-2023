use std::collections::HashMap;
use std::fs;

#[derive(Clone, Debug)]
struct Label {
    label: String,
    box_: u16,
    operand: char,
    focal_length: i32,
}

fn hash_char(c: char, mut hash: u16) -> u16 {
    hash += c as u16;
    hash = hash.wrapping_mul(17);
    hash %= 256;
    hash
}

fn hash_string(s: &str, mut hash: u16) -> u16 {
    for c in s.chars() {
        hash = hash_char(c, hash);
    }
    hash
}

fn assign_lens(label: Label, hash_map: &mut HashMap<u16, Vec<Label>>) {
    match label.operand {
        '-' => {
            if let Some(labels) = hash_map.get_mut(&label.box_) {
                labels.retain(|l| l.label != label.label);
            }
        }
        '=' => {
            let labels = hash_map.entry(label.box_).or_insert_with(Vec::new);
            if let Some(existing_label) = labels.iter_mut().find(|l| l.label == label.label) {
                existing_label.focal_length = label.focal_length;
            } else {
                labels.push(label);
            }
        }
        _ => panic!("Invalid operand"),
    }
}

fn parse_label(complete_label: &str) -> Label {
    let operand = if complete_label.contains('=') {
        '='
    } else {
        '-'
    };
    let parts: Vec<&str> = complete_label.split(operand).collect();
    let label = parts[0].to_string();
    let focal_length = parts.get(1).and_then(|s| s.parse().ok()).unwrap_or(0);
    let box_ = hash_string(&label, 0);
    Label {
        label,
        box_,
        operand,
        focal_length,
    }
}

fn calc_focusing_power(input: &str) -> i32 {
    let labels: Vec<&str> = input.split(',').collect();
    let mut hash_map: HashMap<u16, Vec<Label>> = HashMap::new();

    for label in labels {
        let parsed_label = parse_label(label);
        assign_lens(parsed_label, &mut hash_map);
    }

    let mut focusing_power = 0;
    for (box_, labels) in hash_map {
        if labels.is_empty() {
            continue;
        }
        for (index, label) in labels.iter().enumerate() {
            let box_value = (box_ as i32) + 1;
            let index_value = (index as i32) + 1;
            focusing_power += box_value * index_value * label.focal_length;
        }
    }

    focusing_power
}

fn main() {
    let input = fs::read_to_string("/Users/carlosgonzalez/personal/advent-of-code-2023/day-15/input.txt").expect("Unable to read file");
    let input = input.trim();

    let start = std::time::Instant::now();
    let response = calc_focusing_power(&input);
    println!("Focusing power: {}", response);
    println!("Time taken: {}ms", start.elapsed().as_millis());
}
