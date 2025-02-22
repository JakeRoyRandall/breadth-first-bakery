#include <assert.h>
#include <stdio.h>
#define BAKERY_TEST
#include "game.c"

int main(void) {
    Game g;
    game_init_seed(&g, 7);
    assert(move_player(&g, 'd') == 1);
    assert(g.player.col == 2 && undo_move(&g) == 1);
    assert(g.player.row == 1 && g.player.col == 1);
    assert(undo_move(&g) == 0);

    int row = g.player.row;
    int col = g.player.col;
    assert(move_player(&g, 'x') == 0);
    assert(g.player.row == row && g.player.col == col);

    game_init_seed(&g, 7);
    for (int i = 0; i < 12; i++) assert(move_player(&g, 'd') == 1);
    assert(g.delivered == 1 && g.player.col == g.bakery.col);
    assert(undo_move(&g) == 1 && g.delivered == 0 && g.player.col == g.bakery.col - 1);

    game_init_seed(&g, 7);
    for (int i = 0; i < 21; i++) {
        assert(move_player(&g, (i % 2 == 0) ? 'd' : 'a') == 1);
    }
    for (int i = 0; i < 20; i++) assert(undo_move(&g) == 1);
    assert(undo_move(&g) == 0);

    game_init_seed(&g, 7);
    assert(move_player(&g, 'd') == 1);
    assert(save_game(&g, "/tmp/bakery-undo-save.txt"));
    assert(move_player(&g, 'd') == 1);
    assert(load_game(&g, "/tmp/bakery-undo-save.txt"));
    assert(undo_move(&g) == 0);
    remove("/tmp/bakery-undo-save.txt");

    game_init_seed(&g, 7);
    assert(move_player(&g, 'd') == 1);
    game_init_seed(&g, 7);
    assert(undo_move(&g) == 0);
    puts("undo tests passed: reversal, invalid move, delivery state, cap, load/reset clearing");
    return 0;
}
