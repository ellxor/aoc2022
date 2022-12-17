#![feature(let_chains)]
use std::convert::TryInto;

type Rock = u32;

const ROCKS: [Rock; 5] = [
     0b0111100,

    (0b0001000 << 16) +
    (0b0011100 <<  8) +
     0b0001000,

    (0b0010000 << 16) +
    (0b0010000 <<  8) +
     0b0011100,

    (0b0000100 << 24) +
    (0b0000100 << 16) +
    (0b0000100 <<  8) +
     0b0000100,

    (0b0001100 <<  8) +
     0b0001100,
];

// run simulation
fn drop_rock(index: usize, tower: &mut Vec<u8>, drift: &[u8], mut drift_index: usize) -> usize {
    let mut rock = ROCKS[index];

    let gust = |rock, dir| match dir {
         b'>' => if rock & 0x40404040 != 0 { rock } else { rock << 1 },
         b'<' => if rock & 0x01010101 != 0 { rock } else { rock >> 1 },
         _    => unreachable!(),
    };

    let mut height = tower.len();

    // add space for rock to fall
    for _ in 0..4 {
        tower.push(0);
        rock = gust(rock, drift[drift_index]);
        drift_index = (drift_index + 1) % drift.len();
    }

    // simulate rock falling
    while height != 0 {
        let next = u32::from_le_bytes(tower[height-1..height+3].try_into().unwrap());

        if rock & next != 0 { break; }
        height -= 1;

        let gusted_rock = gust(rock, drift[drift_index]);
        drift_index = (drift_index + 1) % drift.len();

        if gusted_rock & next == 0 { rock = gusted_rock };
    }
 
    // add rock to tower
    for h in height..height+4 {
        tower[h] |= (rock & 0xff) as u8;
        rock >>= 8;
    }

    // remove blank rows from tower
    while let Some(&x) = tower.last() && x == 0 {
        tower.pop();
    }

    return drift_index;
}

fn solve(input: &str) -> (usize, usize) {
    let wind = &input.as_bytes()[..input.len() - 1]; // remove trailing newline

    let mut tower = Vec::<u8>::new();
    let mut index = 0;

    let mut seen = std::collections::HashMap::<(usize,usize,usize), (usize,usize)>::new();
    let mut history = Vec::new();

    for i in 0.. {
        index = drop_rock(i % 5, &mut tower, wind, index);

        if tower.len() >= 8 {
            let last8 = usize::from_le_bytes(tower[tower.len() - 8..].try_into().unwrap());
            let key = (i % 5, index, last8);

            if seen.contains_key(&key) { 
                let (last_index, last_height) = seen.get(&key).unwrap();

                let cycle_length = i - last_index;
                let cycle_height = tower.len() - last_height;

                let solve = |final_index| {
                    let cycles = (final_index - last_index - 1) / cycle_length;
                    let remainder = (final_index - last_index - 1) % cycle_length;

                    let height = cycle_height * cycles + history[last_index + remainder];
                    return height;
                };

                return (solve(2022), solve(1000000000000));
            }

            seen.insert(key, (i, tower.len()));
        }

        history.push(tower.len());
    }

    unreachable!();
}

fn main() {
    assert_eq!(solve(include_str!("input/day17/sample")), (3068, 1514285714288));

    let start = std::time::Instant::now();
    let (part1, part2) = solve(include_str!("input/day17/sample"));
    let time = start.elapsed().as_secs_f32();

    println!("part 1: {part1}, part 2: {part2}, time: {} μs", (time * 1e6) as usize);
}
