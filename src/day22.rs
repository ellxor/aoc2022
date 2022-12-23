fn solve(input: &str) -> (isize, isize) {
    let (input, mut instr) = input.split_once("\n\n").unwrap();

    let height = input.lines().count();
    let width = input.lines().map(|line| line.len()).max().unwrap();

    let mut grid = vec![vec![' '; width]; height];

    for (y, line) in input.lines().enumerate() {
        for (x, c) in line.chars().enumerate() {
            grid[y][x] = c;
        }
    }

    // start position
    let mut x = grid[0].iter().position(|&tile| tile == '.').expect("No start position!") as isize;
    let mut y = 0;

    // direction
    let mut dx = 1 as isize;
    let mut dy = 0 as isize;

    loop {
        match instr.chars().next().unwrap() {
            '\n' => break,
            'L' => { (dx,dy) = (dy,-dx); instr = &instr[1..]; }, // turn anticlockwise
            'R' => { (dx,dy) = (-dy,dx); instr = &instr[1..]; }, // turn clockwise

            '0'..='9' => {
                let length = instr.chars().take_while(|c| c.is_digit(10)).count();
                let (step, rest) = instr.split_at(length);

                let step = step.parse::<usize>().unwrap();
                instr = rest;

                // move forward according to rules
                for _ in 0..step {
                    let mut nx = (x + dx).rem_euclid(width as isize);
                    let mut ny = (y + dy).rem_euclid(height as isize);

                    while grid[ny as usize][nx as usize] == ' ' {
                        nx = (nx + dx).rem_euclid(width as isize);
                        ny = (ny + dy).rem_euclid(height as isize);
                    }

                    if grid[ny as usize][nx as usize] == '#' {
                        break;
                    }

                    x = nx;
                    y = ny;
                }
            },

            _ => unreachable!("Invalid input!"),
        }
    }
    
    let facing = match (dx, dy) {
        (1,0) => 0, (0,1) => 1, (-1,0) => 2, (0,-1) => 3, _ => unreachable!(),
    };

    let part1 = 1000 * (y + 1) + 4 * (x + 1) + facing;

    return (part1, 0);
}

fn main() {
    let clock = std::time::Instant::now();
    let (part1, part2) = solve(include_str!("input/day22/input"));
    let time = clock.elapsed().as_secs_f64();

    println!("{part1} {part2} {time}");
}
