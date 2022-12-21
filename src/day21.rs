#![allow(unused_variables)] // prevent compiler bug (complains about unused variables that are
                            // actually used.
use std::convert::TryInto;

type ID = u32;
type State = std::collections::HashMap<ID, Var>;

#[derive(Clone, Copy)]
enum Op { Add, Sub, Mul, Div }
#[derive(Clone, Copy)]
enum Var { Calc(ID, ID, Op), Val(u64) }

fn parse_id(text: &str) -> ID {
    return ID::from_le_bytes(text.as_bytes().try_into().unwrap());
}

fn parse_op(text: &str) -> Op {
    return match text.chars().nth(0).unwrap() {
        '+' => Op::Add, '-' => Op::Sub, '*' => Op::Mul, '/' => Op::Div, _ => unreachable!(),
    };
}

fn parse(line: &str) -> (ID, Var) {
    let (id, line) = line.split_once(':').unwrap();
    let id = parse_id(id);

    let line = &line[1..];
    
    match line.chars().nth(0).unwrap() {
        '0'..='9' => return (id, Var::Val(line.parse().unwrap())),
        _         => {
            let (arg1, line) = line.split_once(' ').unwrap();
            let (op, arg2) = line.split_once(' ').unwrap();

            return (id, Var::Calc(parse_id(arg1), parse_id(arg2), parse_op(op)));
        }
    }
}

fn part1(id: ID, state: &mut State, trail: &mut Vec<ID>) -> (u64, bool) {
    return match state[&id] {
        Var::Val(x) => (x, id == parse_id("humn")),
        Var::Calc(a, b, op) => {
            let (x, t1) = part1(a, state, trail);
            let (y, t2) = part1(b, state, trail);

            if t1 { trail.push(a) }
            if t2 { trail.push(b) }

            let z = match op {
                Op::Add => x + y, Op::Sub => x - y, Op::Mul => x * y, Op::Div => x / y
            };

            state.insert(id, Var::Val(z));
            return (z, t1 | t2);
        }
    }
}

fn part2(id: ID, state: &State, equations: &State, trail: &[ID], expected: u64) -> u64 {
    if id == parse_id("humn") { return expected; }

    let Var::Calc(a, b, op) = equations[&id] else { unreachable!() };

    let is_left = a == trail[0];
    let Var::Val(arg) = (if is_left { state[&b] } else { state[&a] }) else { unreachable!() };

    let next = match (op, is_left) {
        (Op::Add, _    ) => expected - arg,
        (Op::Sub, true ) => arg + expected,
        (Op::Sub, false) => arg - expected,
        (Op::Mul, _    ) => expected / arg,
        (Op::Div, true ) => arg * expected,
        (Op::Div, false) => arg / expected,
    };

    return part2(trail[0], state, equations, &trail[1..], next);
}

fn solve(input: &str) -> (u64, u64) {
    let mut equations: State = input.lines().map(parse).collect();
    let mut state: State = equations.clone();
    let mut trail = Vec::<ID>::new();

    let root = parse_id("root");
    let (part1, _) = part1(root, &mut state, &mut trail);

    // modify root to be of the form x - y == 0
    let Var::Calc(a,b,op) = equations[&root] else { unreachable!() };
    equations.insert(root, Var::Calc(a, b, Op::Sub));

    trail.reverse();
    let part2 = part2(root, &state, &equations, &trail, 0);

    return (part1, part2);
}

fn main() {
    assert_eq!(solve(include_str!("input/day21/sample")), (152,301));

    let clock = std::time::Instant::now();
    let (part1, part2) = solve(include_str!("input/day21/input"));
    let time = clock.elapsed().as_secs_f64();

    println!("part 1: {part1}, part 2: {part2}, time: {:.0} μs", time * 1e6);
}
