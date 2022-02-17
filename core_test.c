#include <assert.h>
#include <stdio.h>
#define BAKERY_TEST
#include "game.c"
int main(void) {
    Game g; game_init(&g);
    assert(g.player.row == 1 && g.player.col == 1);
    assert(g.map[g.player.row][g.player.col] == ' ');
    assert(shortest_path(&g) > 0);
    assert(move_player(&g, 'd') == 1);
    assert(move_player(&g, 'w') == 0);
    assert(move_player(&g, 'x') == 0);
    Game hinted = g;
    char direction = next_direction(&hinted);
    assert(direction == 'd');
    assert(hinted.player.row == g.player.row && hinted.player.col == g.player.col && hinted.moves == g.moves && hinted.undo_count == g.undo_count);
    for (unsigned seed = 7; seed <= 8; seed++) {
        game_init_seed(&hinted, seed);
        for (int step = 0; step < 200 && !hinted.delivered; step++) {
            int before = shortest_path(&hinted);
            char next = next_direction(&hinted);
            assert(next == 'w' || next == 'a' || next == 's' || next == 'd');
            assert(move_player(&hinted, next) == 1);
            assert(shortest_path(&hinted) < before);
        }
        assert(hinted.delivered == 1);
    }
    puts("core tests passed: start bounds, blocked movement, reachable BFS, invalid command");
}
