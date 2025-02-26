# Breadth-First Bakery

A small terminal sourdough delivery maze set in a fictional 2020. Created September 2026 retrospectively; this is a fictional 2020-inspired project, not a historical work record. No medical claims or advice are involved.

Build and run the final snapshot:

```sh
clang -std=c11 -Wall -Wextra -pedantic game.c -o bakery
./bakery 8
```

Use `w/a/s/d` to move, `h` for a shortest-path hint, `u` to undo a successful move, `p` to save, `l` to load, `r` to reset, and `q` to quit. Undo keeps up to 20 prior positions and restores the maze, player, bakery, and delivery state. Hints and invalid or informational commands do not add history; reset and successful load clear it. Run the compiled checks from the project root with `clang -std=c11 -Wall -Wextra -pedantic core_test.c -o /tmp/bakery-core-test && /tmp/bakery-core-test`, and similarly for `feature_test.c`, `save_test.c`, and `undo_test.c`.

Successful moves are counted, undo restores the prior count, and restart clears it. Current saves add the move count as a sixth header integer; legacy five-field saves load with zero moves. The count is limited to 1,000,000; further moves leave the game unchanged. Malformed headers fail without replacing the current game.

The `n` command suggests the next key along a shortest route. Ties prefer north, east, south, then west. It changes no state, counter, or undo history; `h` still reports remaining distance.

Saves use an exclusively created temporary sibling, then rename only after all writes and close succeed. Failed saves preserve the existing target and clean up temporary output. This implementation targets POSIX systems such as macOS and Linux.
