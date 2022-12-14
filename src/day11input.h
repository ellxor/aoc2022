#pragma once

const char *input =
// "Monkey 0:\n"
// "  Starting items: 79, 98\n"
// "  Operation: new = old * 19\n"
// "  Test: divisible by 23\n"
// "    If true: throw to monkey 2\n"
// "    If false: throw to monkey 3\n"
// "\n"
// "Monkey 1:\n"
// "  Starting items: 54, 65, 75, 74\n"
// "  Operation: new = old + 6\n"
// "  Test: divisible by 19\n"
// "    If true: throw to monkey 2\n"
// "    If false: throw to monkey 0\n"
// "\n"
// "Monkey 2:\n"
// "  Starting items: 79, 60, 97\n"
// "  Operation: new = old * old\n"
// "  Test: divisible by 13\n"
// "    If true: throw to monkey 1\n"
// "    If false: throw to monkey 3\n"
// "\n"
// "Monkey 3:\n"
// "  Starting items: 74\n"
// "  Operation: new = old + 3\n"
// "  Test: divisible by 17\n"
// "    If true: throw to monkey 0\n"
// "    If false: throw to monkey 1\n"
// "\n";

"Monkey 0:\n"
"  Starting items: 85, 79, 63, 72\n"
"  Operation: new = old * 17\n"
"  Test: divisible by 2\n"
"    If true: throw to monkey 2\n"
"    If false: throw to monkey 6\n"
"\n"
"Monkey 1:\n"
"  Starting items: 53, 94, 65, 81, 93, 73, 57, 92\n"
"  Operation: new = old * old\n"
"  Test: divisible by 7\n"
"    If true: throw to monkey 0\n"
"    If false: throw to monkey 2\n"
"\n"
"Monkey 2:\n"
"  Starting items: 62, 63\n"
"  Operation: new = old + 7\n"
"  Test: divisible by 13\n"
"    If true: throw to monkey 7\n"
"    If false: throw to monkey 6\n"
"\n"
"Monkey 3:\n"
"  Starting items: 57, 92, 56\n"
"  Operation: new = old + 4\n"
"  Test: divisible by 5\n"
"    If true: throw to monkey 4\n"
"    If false: throw to monkey 5\n"
"\n"
"Monkey 4:\n"
"  Starting items: 67\n"
"  Operation: new = old + 5\n"
"  Test: divisible by 3\n"
"    If true: throw to monkey 1\n"
"    If false: throw to monkey 5\n"
"\n"
"Monkey 5:\n"
"  Starting items: 85, 56, 66, 72, 57, 99\n"
"  Operation: new = old + 6\n"
"  Test: divisible by 19\n"
"    If true: throw to monkey 1\n"
"    If false: throw to monkey 0\n"
"\n"
"Monkey 6:\n"
"  Starting items: 86, 65, 98, 97, 69\n"
"  Operation: new = old * 13\n"
"  Test: divisible by 11\n"
"    If true: throw to monkey 3\n"
"    If false: throw to monkey 7\n"
"\n"
"Monkey 7:\n"
"  Starting items: 87, 68, 92, 66, 91, 50, 68\n"
"  Operation: new = old + 2\n"
"  Test: divisible by 17\n"
"    If true: throw to monkey 4\n"
"    If false: throw to monkey 3\n"
"\n";