// store resources (for both materials materials and robots in a u64 as a u16x4) using bitwise operators
// the order is in little-endian (lsb: ore, clay, obsidian, geode) 
const ORE_MASK:      u64 = 0x000000000000FFFF;
const CLAY_MASK:     u64 = 0x00000000FFFF0000;
const OBSIDIAN_MASK: u64 = 0x0000FFFF00000000;
const GEODE_MASK:    u64 = 0xFFFF000000000000;

#[derive(Debug)]
struct Blueprint { ore: u64, clay: u64, obsidian: u64, geode: u64 }

fn parse(line: &str) -> Blueprint {
    let (_, line) = line.split_once(':').unwrap();

    let length = " Each ore robot costs ".len();
    let line = &line[length..];
    
    let (ore,line) = line.split_once(' ').unwrap();

    let length = "ore. Each clay robot costs ".len();
    let line = &line[length..];

    let (clay, line) = line.split_once(' ').unwrap();

    let length = "ore. Each obsidian robot costs ".len();
    let line = &line[length..];

    let (obsidian_ore, line) = line.split_once(' ').unwrap();

    let length = "ore and ".len();
    let line = &line[length..];

    let (obsidian_clay, line) = line.split_once(' ').unwrap();

    let length = "clay. Each geode robot costs ".len();
    let line = &line[length..];

    let (geode_ore, line) = line.split_once(' ').unwrap();

    let length = "ore and ".len();
    let line = &line[length..];

    let (geode_obsidian, _) = line.split_once(' ').unwrap();

    return Blueprint {
        ore:      ore.parse().unwrap(),
        clay:     clay.parse().unwrap(),
        obsidian: obsidian_ore.parse::<u64>().unwrap() + (obsidian_clay.parse::<u64>().unwrap() << 16),
        geode:    geode_ore.parse::<u64>().unwrap() + (geode_obsidian.parse::<u64>().unwrap() << 32)
    };
}

// TODO: code logic can do with a lot of clean-up
fn dfs(bp: &Blueprint, materials: u64, robots: u64, time: u64, skip: u64, alpha: u64) -> u64 {
    // shortcut last branch .. building a new robot on last step does nothing
    if time == 1 { return (materials + robots) >> GEODE_MASK.trailing_zeros(); }

    // check material requirement to build robot
    let check = |req,mask| (materials & mask) >= (req & mask);

    // calculate theoretical best of a material with state
    let optimistic_best = |mask: u64| {
        let m = (materials & mask) >> mask.trailing_zeros();
        let r = (robots & mask) >> mask.trailing_zeros();
        return m + (r * time) + (time * (time - 1) / 2);
    };

    // prune branch if we cannot mine enough geodes
    if optimistic_best(GEODE_MASK) < alpha {
        return 0;
    } 

    // prune branch if we cannot make geode robots
    if optimistic_best(OBSIDIAN_MASK) < (bp.geode >> OBSIDIAN_MASK.trailing_zeros()) {
        return (materials >> GEODE_MASK.trailing_zeros()) + (robots >> GEODE_MASK.trailing_zeros()) * time;
    }

    // if a geode robot can be built, then it is always optimal
    if check(bp.geode, ORE_MASK) && check(bp.geode, OBSIDIAN_MASK) {
        return dfs(bp, materials + robots - bp.geode, robots + (1 << GEODE_MASK.trailing_zeros()), time - 1, 0, alpha);
    }

    // otherwise check all options
    let mut value = alpha;

    let max_ore = [bp.ore, bp.clay, bp.obsidian, bp.geode].iter().map(|x| x & ORE_MASK).max().unwrap();
    let max_clay = bp.obsidian & CLAY_MASK;
    let max_obsidian = bp.geode & OBSIDIAN_MASK;

    let mut available = 0;

    if check(bp.obsidian, ORE_MASK) && check(bp.obsidian, CLAY_MASK) && (robots & OBSIDIAN_MASK) < max_obsidian && (skip & OBSIDIAN_MASK) == 0 {
        let next = dfs(bp, materials + robots - bp.obsidian, robots + (1 << OBSIDIAN_MASK.trailing_zeros()), time - 1, 0, value);
        value = std::cmp::max(value, next);
        available |= OBSIDIAN_MASK;
    }

    if (materials & ORE_MASK) >= bp.clay && (robots & CLAY_MASK) < max_clay && (skip & CLAY_MASK) == 0 {
        let next = dfs(bp, materials + robots - bp.clay, robots + (1 << CLAY_MASK.trailing_zeros()), time - 1, 0, value);
        value = std::cmp::max(value, next);
        available |= CLAY_MASK;
    }

    if (materials & ORE_MASK) >= bp.ore && (robots & ORE_MASK) < max_ore && (skip & ORE_MASK) == 0 {
        let next = dfs(bp, materials + robots - bp.ore, robots + 1, time - 1, 0, value);
        value = std::cmp::max(value, next);
        available |= ORE_MASK;
    }

    // null pruning is least effective
    let null = dfs(bp, materials + robots, robots, time - 1, available, value);
    return std::cmp::max(value, null);
}

fn solve(input: &str) -> (u64,u64) {
    let blueprints = input.lines().map(parse).collect::<Vec<Blueprint>>();

    let part1 = blueprints.iter()
        .enumerate()
        .map(|(i,bp)| (i+1) as u64 * dfs(bp, 0, 1, 24, 0, 0))
        .sum();

    let part2 = blueprints.iter()
        .take(3)
        .map(|bp| dfs(bp, 0, 1, 32, 0, 0))
        .product();

    return (part1, part2);
}

fn main() {
    let start = std::time::Instant::now();
    let (part1, part2) = solve(include_str!("input/day19/input"));
    let time = start.elapsed().as_secs_f64();

    println!("part 1: {part1}, part 2: {part2}, time: {:.0} µs", time * 1e6);
}
