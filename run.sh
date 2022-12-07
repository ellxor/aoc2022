set -xe

CFLAGS="-O3 -march=native -w"

clang $CFLAGS day01.c && ./a.out
clang $CFLAGS day02.c && ./a.out
clang $CFLAGS day03.c && ./a.out
clang $CFLAGS day04.c && ./a.out
clang $CFLAGS day05.c && ./a.out
clang $CFLAGS day06.c && ./a.out
clang $CFLAGS day07.c && ./a.out

rm -rf a.out
