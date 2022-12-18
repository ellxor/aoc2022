#[derive(Debug)]
struct Room {
    id: usize,
    flow_rate: usize,
    tunnels: Vec<usize>,
}

fn parse_id(input: &str) -> usize {
    let bytes = input.as_bytes();
    return (bytes[0] as usize) + ((bytes[1] as usize) << 8);
}

fn parse(input: &str) -> Room {
    let input = &input[6..];
    let id = parse_id(input);
    let input = &input[17..];

    let (flow_rate, input) = input.split_once(';').expect("Invalid input!");
    let flow_rate = flow_rate.parse().expect("Invalid input!");

    let input = &input[22..];
    let (_, input) = input.split_once(' ').expect("Invalid input!");

    return Room {
        id: id,
        flow_rate: flow_rate,
        tunnels: input.split(", ").map(parse_id).collect(),
    };
}

type Cache = std::collections::HashMap<(usize, usize, usize, bool), usize>;

// use depth first search to find maximal solution
fn dfs(i: usize, aa: usize, elephant: bool, time: usize, visit: usize, rooms: &Vec<Room>, dist: &[[usize; 64]; 64], cache: &mut Cache) -> usize {
    let args = (i, time, visit, elephant);

    if cache.contains_key(&args) {
        return *cache.get(&args).unwrap();
    }

    if time <= 2 {
        return if elephant { dfs(aa, aa, false, 26, visit, rooms, dist, cache) } else { 0 };
    }

    let mut res = 0;
    let mut mask = visit;

    while mask != 0 {
        let j = mask.trailing_zeros() as usize;

        if dist[i][j] < time {
            let remainder = time - dist[i][j] - 1;
            res = std::cmp::max(res, remainder * rooms[j].flow_rate + dfs(j, aa, elephant, remainder, visit &! (1 << j), rooms, dist, cache));
        }

        mask &= mask - 1;
    }

    cache.insert(args, res);
    return res;
}

fn solve(input: &str) -> (usize, usize) {
    // parse input into rooms
    let rooms = input.lines()
        .map(parse)
        .collect::<Vec<Room>>();

    // normalise tunnel IDs to indices
    let locate_id = |&id| rooms.iter()
        .position(|room| room.id == id)
        .expect("Invalid ID in input");

    let rooms = rooms.iter()
        .map(|room| Room {
            tunnels: room.tunnels
                .iter()
                .map(locate_id)
                .collect(),
            ..*room
        })
        .collect::<Vec<Room>>();

    // use floyd-warshall algorithm to create adjacency distance matrix
    // initialise to infinity (255 in this case)
    let mut dist = [[255; 64]; 64];

    for i in 0..rooms.len() {
        for &j in &rooms[i].tunnels {
            dist[i][j] = 1;
        }
    }

    for k in 0..rooms.len() {
        for i in 0..rooms.len() {
            for j in 0..rooms.len() {
                dist[i][j] = std::cmp::min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    // construct bitmask of nodes to visit
    let visit_mask: usize = rooms.iter()
        .enumerate()
        .filter(|(_,room)| room.flow_rate != 0)
        .map(|(i,_)| 1 << i)
        .sum();

    let start = rooms.iter()
        .position(|room| room.id == parse_id("AA"))
        .expect("Room with ID 'AA' must exist!");

    let mut cache = Cache::new();

    let part1 = dfs(start, start, false, 30, visit_mask, &rooms, &dist, &mut cache);
    let part2 = dfs(start, start, true, 26, visit_mask, &rooms, &dist, &mut cache);

    return (part1, part2);
}

fn main() {
    let start = std::time::Instant::now();
    let (part1, part2) = solve(include_str!("input/day16/input"));
    let time = start.elapsed().as_secs_f64();

    println!("Part 1: {}\nPart 2: {}\nTime: {:.3} ms", part1, part2, time * 1e3);
}
