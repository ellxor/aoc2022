#[derive(Debug)]
struct Cube(usize,usize,usize);

fn parse(input: &str) -> Vec<Cube> {
    return input.lines()
                .map(|line| {
                    let (x, line) = line.split_once(',').unwrap();
                    let (y, z) = line.split_once(',').unwrap();
                    return Cube(
                        x.parse::<usize>().expect("Invalid input!") + 1,
                        y.parse::<usize>().expect("Invalid input!") + 1,
                        z.parse::<usize>().expect("Invalid input!") + 1);
                }).collect();
}

fn flood_fill(grid: &mut [[[u8; 24]; 24]; 24], x: usize, y: usize, z: usize) -> u64 {
    return match grid[x][y][z] {
        1 => 1,
        2 => 0,
        0 => {
            grid[x][y][z] = 2;
            let mut surface_area = 0;

            if x !=  0 { surface_area += flood_fill(grid, x - 1, y, z); }
            if x != 23 { surface_area += flood_fill(grid, x + 1, y, z); }
            if y !=  0 { surface_area += flood_fill(grid, x, y - 1, z); }
            if y != 23 { surface_area += flood_fill(grid, x, y + 1, z); }
            if z !=  0 { surface_area += flood_fill(grid, x, y, z - 1); }
            if z != 23 { surface_area += flood_fill(grid, x, y, z + 1); }

            return surface_area;
        },
        _ => unreachable!()
    }
}

fn solve(input: &str) -> (u64, u64) {
    let cubes = parse(input); 
    let mut grid = [[[0 as u8; 24]; 24]; 24];

    for cube in &cubes {
        let Cube(x,y,z) = *cube;
        grid[x][y][z] = 1;
    }

    let mut part1 = 0;

    for cube in &cubes {
        let Cube(x,y,z) = *cube;
        let faces = 6 - grid[x-1][y][z] - grid[x+1][y][z]
                      - grid[x][y-1][z] - grid[x][y+1][z]
                      - grid[x][y][z-1] - grid[x][y][z+1];

        part1 += faces as u64;
    }

    let part2 = flood_fill(&mut grid, 0, 0, 0);
    return (part1, part2);
}

fn main() {
    assert_eq!(solve(include_str!("input/day18/sample")), (64, 58));

    let start = std::time::Instant::now();
    let (part1, part2) = solve(include_str!("input/day18/input"));
    let time = start.elapsed().as_secs_f64();

    println!("part 1: {part1}, part 2: {part2}, time: {:.0} μs", time * 1e6);
}
