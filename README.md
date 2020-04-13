# Breadth-First Bakery — 03-feature

Created September 2026 retrospectively; fictional 2020-inspired project, not a historical work record. Build with `clang -std=c11 -Wall -Wextra -pedantic game.c -o bakery` and run `./bakery [seed]`. Use `p` to save to `bakery.save`, `l` to load it, and `h` for the BFS distance hint. The root test command compiles `tests/save_test.c` against this snapshot.

Run the compiled checks: `clang -std=c11 -Wall -Wextra -pedantic test_game.c -o /tmp/bakery-test && /tmp/bakery-test`. Tests use temporary named files in /tmp.

Git author dates are deliberately assigned for contribution-calendar artwork, not claims of historical work. Committer timestamps record actual creation in September 2026.

Seeds select one of two hand-authored layouts by parity; this is not procedural maze generation. `h` reports the shortest remaining distance, not the route.
