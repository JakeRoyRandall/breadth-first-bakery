#include <assert.h>
#include <stdio.h>
#define BAKERY_TEST
#include "game.c"
int main(void) {
    Game before, after; game_init_seed(&before, 8); move_player(&before, 'd');
    assert(save_game(&before, "/tmp/bakery-save-test.txt"));
    assert(load_game(&after, "/tmp/bakery-save-test.txt"));
    assert(after.player.row == before.player.row && after.player.col == before.player.col);
    assert(after.moves == before.moves);
    assert(shortest_path(&after) == shortest_path(&before));
    FILE *legacy = fopen("/tmp/bakery-legacy-save.txt", "w"); assert(legacy);
    fprintf(legacy, "%d %d %d %d %d\n", before.player.row, before.player.col, before.bakery.row, before.bakery.col, before.delivered);
    for (int r = 0; r < H; r++) fprintf(legacy, "%.*s\n", W, before.map[r]);
    fclose(legacy);
    assert(load_game(&after, "/tmp/bakery-legacy-save.txt") && after.moves == 0);
    remove("/tmp/bakery-legacy-save.txt");
    Game preserved = after;
    const char *bad_headers[] = { "1 2 7 13 0 junk\n", "1 2 7 13 0 -1\n", "1 2 7 13 0 1000001\n" };
    for (size_t i = 0; i < sizeof(bad_headers) / sizeof(bad_headers[0]); i++) {
        FILE *bad_header = fopen("/tmp/bakery-bad-header.txt", "w"); assert(bad_header);
        fputs(bad_headers[i], bad_header);
        for (int r = 0; r < H; r++) fprintf(bad_header, "%.*s\n", W, before.map[r]);
        fclose(bad_header);
        assert(!load_game(&after, "/tmp/bakery-bad-header.txt"));
        assert(after.player.row == preserved.player.row && after.player.col == preserved.player.col && after.moves == preserved.moves);
    }
    remove("/tmp/bakery-bad-header.txt");
    FILE *bad = fopen("/tmp/bakery-bad-save.txt", "w"); assert(bad); fputs("1 2 7 13 0\n###############\n", bad); fclose(bad);
    assert(!load_game(&after, "/tmp/bakery-bad-save.txt"));
    assert(after.player.row == preserved.player.row && after.player.col == preserved.player.col);
    puts("save tests passed: compiled save/load round trip preserves state and path");
}
