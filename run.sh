set -xe

CFLAGS="-O3 -march=native -flto -w"

clang $CFLAGS day01.c && ./a.out
clang $CFLAGS day02.c && ./a.out
clang $CFLAGS day03.c && ./a.out
clang $CFLAGS day04.c && ./a.out
clang $CFLAGS day05.c && ./a.out
clang $CFLAGS day06.c && ./a.out
clang $CFLAGS day07.c && ./a.out
clang $CFLAGS day08.c && ./a.out
clang $CFLAGS day09.c && ./a.out
clang $CFLAGS day10.c && ./a.out
clang $CFLAGS day11.c && ./a.out

rm -rf a.out
