# Breadth-First Bakery

A small terminal sourdough delivery maze set in a fictional 2020. Created September 2026 retrospectively; this is a fictional 2020-inspired project, not a historical work record. No medical claims or advice are involved.

Build and run the final snapshot:

```sh
clang -std=c11 -Wall -Wextra -pedantic game.c -o bakery
./bakery 8
```

Use `w/a/s/d` to move, `h` for a shortest-path hint, `u` to undo a successful move, `p` to save, `l` to load, `r` to reset, and `q` to quit. Undo keeps up to 20 prior positions and restores the maze, player, bakery, and delivery state. Hints and invalid or informational commands do not add history; reset and successful load clear it. Run the compiled checks from the project root with `clang -std=c11 -Wall -Wextra -pedantic core_test.c -o /tmp/bakery-core-test && /tmp/bakery-core-test`, and similarly for `feature_test.c`, `save_test.c`, and `undo_test.c`.
