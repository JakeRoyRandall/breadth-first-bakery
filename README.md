# Breadth-First Bakery — 01-core

Created September 2026 retrospectively; fictional 2020-inspired project, not a historical work record. Build with `clang -std=c11 -Wall -Wextra -pedantic game.c -o bakery` and run `./bakery`. Commands are line input: `w/a/s/d` move, `h` gives a BFS distance hint, `r` resets, `q` quits. The root test command compiles `tests/core_test.c` against this snapshot.

Run the compiled checks: `clang -std=c11 -Wall -Wextra -pedantic test_game.c -o /tmp/bakery-test && /tmp/bakery-test`. Tests use temporary named files in /tmp.

Git author dates are deliberately assigned for contribution-calendar artwork, not claims of historical work. Committer timestamps record actual creation in September 2026.
