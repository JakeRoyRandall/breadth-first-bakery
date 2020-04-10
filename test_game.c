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
    puts("core tests passed: start bounds, blocked movement, reachable BFS, invalid command");
}
