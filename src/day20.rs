type Node = (i64, bool);

fn part1(input: &str) -> i64 {
    let mut ring = input.lines()
        .map(|x| (x.parse().unwrap(), false))
        .collect::<Vec<Node>>();

    // begin mixing shuffle
    let mut index = 0;
    let length = ring.len();

    while index < length {
        let (value, moved) = ring[index];
        if moved { index += 1; continue; }

        let wrap = length as i64 - 1; // mod length - 1 as item is removed from list
        let new_index = ((index as i64) + value).rem_euclid(wrap) as usize;

        ring.remove(index);
        ring.insert(new_index, (value, true));
    }

    let zero = ring.iter().position(|&(v,_)| v == 0).expect("Input must contain 0");

    return ring[(zero + 1000) % length].0
         + ring[(zero + 2000) % length].0
         + ring[(zero + 3000) % length].0;
}

fn part2(input: &str) -> i64 {
    const KEY: i64 = 811589153;
    const ROUNDS: usize = 10;

    let mut ring = input.lines()
        .map(|x| (KEY * x.parse::<i64>().unwrap()))
        .enumerate()
        .collect::<Vec<(usize,i64)>>();

    let length = ring.len();

    for _ in 0..ROUNDS {
        for original in 0..length {
            let index = ring.iter().position(|&(i,_)| i == original).unwrap();
            let (_, value) = ring[index];

            let wrap = length as i64 - 1;
            let new_index = ((index as i64) + value).rem_euclid(wrap) as usize;

            ring.remove(index);
            ring.insert(new_index, (original, value));
        }
    }

    let zero = ring.iter().position(|&(_,v)| v == 0).expect("Input must contain 0");

    return ring[(zero + 1000) % length].1
         + ring[(zero + 2000) % length].1
         + ring[(zero + 3000) % length].1;
}

fn solve(input: &str) -> (i64, i64) {
    return (part1(input), part2(input));
}

fn main() {
    assert_eq!(solve(include_str!("input/day20/sample")), (3,1623178306));

    let clock = std::time::Instant::now();
    let (part1, part2) = solve(include_str!("input/day20/input"));
    let time = clock.elapsed().as_secs_f64();

    println!("part 1: {part1}, part 2: {part2}, time: {:.0} μs", time * 1e6);
}
