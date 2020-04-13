#include <assert.h>
#include <stdio.h>
#define BAKERY_TEST
#include "game.c"
int main(void) {
    Game before, after; game_init_seed(&before, 8); move_player(&before, 'd');
    assert(save_game(&before, "/tmp/bakery-save-test.txt"));
    assert(load_game(&after, "/tmp/bakery-save-test.txt"));
    assert(after.player.row == before.player.row && after.player.col == before.player.col);
    assert(shortest_path(&after) == shortest_path(&before));
    Game preserved = after;
    FILE *bad = fopen("/tmp/bakery-bad-save.txt", "w"); assert(bad); fputs("1 2 7 13 0\n###############\n", bad); fclose(bad);
    assert(!load_game(&after, "/tmp/bakery-bad-save.txt"));
    assert(after.player.row == preserved.player.row && after.player.col == preserved.player.col);
    puts("save tests passed: compiled save/load round trip preserves state and path");
}
