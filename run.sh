set -xe

CFLAGS="-O3 -march=native -flto -w"
RUST_FLAGS="-C opt-level=3 -C target-cpu=native -o a.out"

cd src/

#clang $CFLAGS day01.c && ./a.out
#clang $CFLAGS day02.c && ./a.out
#clang $CFLAGS day03.c && ./a.out
#clang $CFLAGS day04.c && ./a.out
#clang $CFLAGS day05.c && ./a.out
#clang $CFLAGS day06.c && ./a.out
#clang $CFLAGS day07.c && ./a.out
#clang $CFLAGS day08.c && ./a.out
#clang $CFLAGS day09.c && ./a.out
#clang $CFLAGS day10.c && ./a.out
#clang $CFLAGS day11.c && ./a.out
#clang $CFLAGS day12.c && ./a.out
#clang $CFLAGS day13.c && ./a.out
#clang $CFLAGS day14.c && ./a.out
#clang $CFLAGS day15.c && ./a.out
clang $CFLAGS day16.c && ./a.out
rustc $RUST_FLAGS day17.rs && ./a.out

rm -rf a.out
cd ..
