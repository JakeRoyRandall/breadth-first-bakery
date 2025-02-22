#include <assert.h>
#include <stdio.h>
#define BAKERY_TEST
#include "game.c"
int main(void) {
    Game odd, even; game_init_seed(&odd, 7); game_init_seed(&even, 8);
    assert(shortest_path(&odd) > 0 && shortest_path(&even) > 0);
    assert(odd.bakery.row != even.bakery.row);
    assert(odd.map[odd.player.row][odd.player.col] == ' ');
    assert(move_player(&odd, 'x') == 0);
    puts("feature tests passed: seeded layouts, reachable paths, invalid command");
}
