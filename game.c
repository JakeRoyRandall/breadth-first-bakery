/* Breadth-First Bakery / core snapshot. Fictional 2020-inspired terminal game. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>

#define H 9
#define W 15
#define MOVE_LIMIT 1000000U
typedef struct { int row, col; } Point;
typedef struct { char map[H][W + 2]; Point player, bakery; int delivered; unsigned moves; } Snapshot;
typedef struct { char map[H][W + 2]; Point player, bakery; int delivered; unsigned moves; Snapshot undo[20]; int undo_count; } Game;

static const char *core_map[H] = {
    "###############", "#@           B#", "# ### ### ### #",
    "#             #", "# ### ### ### #", "#             #",
    "# ### ### ### #", "#             #", "###############"
};
static const char *alternate_map[H] = {
    "###############", "#@            #", "# ### ### ### #",
    "#             #", "# ### ### ### #", "#             #",
    "# ### ### ### #", "#            B#", "###############"
};

void game_init_seed(Game *g, unsigned seed) {
    memset(g, 0, sizeof(*g));
    const char **layout = (seed % 2 == 0) ? alternate_map : core_map;
    for (int r = 0; r < H; r++) strcpy(g->map[r], layout[r]);
    for (int r = 0; r < H; r++) for (int c = 0; c < W; c++) {
        if (g->map[r][c] == '@') { g->player = (Point){r, c}; g->map[r][c] = ' '; }
        if (g->map[r][c] == 'B') g->bakery = (Point){r, c};
    }
}
void game_init(Game *g) { game_init_seed(g, 7); }

int can_step(const Game *g, Point p) { return p.row > 0 && p.row < H - 1 && p.col > 0 && p.col < W - 1 && g->map[p.row][p.col] != '#'; }

static Snapshot snapshot_from_game(const Game *g) {
    Snapshot snapshot;
    memcpy(snapshot.map, g->map, sizeof(snapshot.map));
    snapshot.player = g->player;
    snapshot.bakery = g->bakery;
    snapshot.delivered = g->delivered;
    snapshot.moves = g->moves;
    return snapshot;
}

static void restore_snapshot(Game *g, const Snapshot *snapshot) {
    memcpy(g->map, snapshot->map, sizeof(g->map));
    g->player = snapshot->player;
    g->bakery = snapshot->bakery;
    g->delivered = snapshot->delivered;
    g->moves = snapshot->moves;
}

static void remember(Game *g, Snapshot snapshot) {
    if (g->undo_count == 20) {
        memmove(&g->undo[0], &g->undo[1], 19 * sizeof(g->undo[0]));
        g->undo_count = 19;
    }
    g->undo[g->undo_count++] = snapshot;
}

int move_player(Game *g, char command) {
    Point next = g->player;
    if (command == 'w') next.row--; else if (command == 's') next.row++;
    else if (command == 'a') next.col--; else if (command == 'd') next.col++;
    else return 0;
    if (!can_step(g, next)) return 0;
    if (g->moves >= MOVE_LIMIT) return 0;
    Snapshot before = snapshot_from_game(g);
    g->player = next;
    if (next.row == g->bakery.row && next.col == g->bakery.col) g->delivered = 1;
    g->moves++;
    remember(g, before);
    return 1;
}

int undo_move(Game *g) {
    if (g->undo_count == 0) return 0;
    g->undo_count--;
    Snapshot previous = g->undo[g->undo_count];
    restore_snapshot(g, &previous);
    return 1;
}

int shortest_path(const Game *g) {
    int distance[H][W], head = 0, tail = 0; Point queue[H * W];
    for (int r = 0; r < H; r++) for (int c = 0; c < W; c++) distance[r][c] = -1;
    distance[g->player.row][g->player.col] = 0; queue[tail++] = g->player;
    while (head < tail) {
        Point p = queue[head++];
        if (p.row == g->bakery.row && p.col == g->bakery.col) return distance[p.row][p.col];
        Point neighbors[4] = {{p.row-1,p.col},{p.row+1,p.col},{p.row,p.col-1},{p.row,p.col+1}};
        for (int i = 0; i < 4; i++) { Point n = neighbors[i]; if (can_step(g,n) && distance[n.row][n.col] < 0) { distance[n.row][n.col] = distance[p.row][p.col] + 1; queue[tail++] = n; } }
    }
    return -1;
}

char next_direction(const Game *g) {
    if (g->delivered || (g->player.row == g->bakery.row && g->player.col == g->bakery.col)) return '!';
    int distance[H][W], head = 0, tail = 0; Point queue[H * W];
    for (int r = 0; r < H; r++) for (int c = 0; c < W; c++) distance[r][c] = -1;
    distance[g->bakery.row][g->bakery.col] = 0; queue[tail++] = g->bakery;
    while (head < tail) {
        Point p = queue[head++];
        Point neighbors[4] = {{p.row-1,p.col},{p.row,p.col+1},{p.row+1,p.col},{p.row,p.col-1}};
        for (int i = 0; i < 4; i++) { Point n = neighbors[i]; if (can_step(g,n) && distance[n.row][n.col] < 0) { distance[n.row][n.col] = distance[p.row][p.col] + 1; queue[tail++] = n; } }
    }
    Point neighbors[4] = {{g->player.row-1,g->player.col},{g->player.row,g->player.col+1},{g->player.row+1,g->player.col},{g->player.row,g->player.col-1}};
    const char directions[4] = {'w', 'd', 's', 'a'};
    for (int i = 0; i < 4; i++) if (can_step(g, neighbors[i]) && distance[neighbors[i].row][neighbors[i].col] >= 0 && distance[neighbors[i].row][neighbors[i].col] < distance[g->player.row][g->player.col]) return directions[i];
    return '?';
}

void draw(const Game *g) {
    for (int r = 0; r < H; r++) { for (int c = 0; c < W; c++) { char tile = g->map[r][c] == '@' ? ' ' : g->map[r][c]; putchar((r == g->player.row && c == g->player.col) ? '@' : tile); } putchar('\n'); }
}

int save_game(const Game *g, const char *path) {
    size_t path_length = strlen(path);
    char *temporary_path = malloc(path_length + 32);
    if (temporary_path == NULL) return 0;
    if (snprintf(temporary_path, path_length + 32, "%s.tmp-%ld", path, (long)getpid()) < 0) { free(temporary_path); return 0; }
    FILE *file = fopen(temporary_path, "wx");
    if (!file) { free(temporary_path); return 0; }
    int ok = fprintf(file, "%d %d %d %d %d %u\n", g->player.row, g->player.col, g->bakery.row, g->bakery.col, g->delivered, g->moves) >= 0;
    for (int r = 0; r < H && ok; r++) ok = fprintf(file, "%.*s\n", W, g->map[r]) >= 0;
    if (fclose(file) != 0) ok = 0;
    if (ok && rename(temporary_path, path) != 0) ok = 0;
    if (!ok) remove(temporary_path);
    free(temporary_path);
    return ok;
}

static int parse_int_token(const char *token, int *value) {
    char *end;
    long parsed;
    errno = 0;
    parsed = strtol(token, &end, 10);
    if (token[0] == '\0' || *end != '\0' || errno == ERANGE || parsed < INT_MIN || parsed > INT_MAX) return 0;
    *value = (int)parsed;
    return 1;
}

static int parse_moves_token(const char *token, unsigned *value) {
    char *end;
    unsigned long parsed;
    errno = 0;
    if (token[0] == '-' || token[0] == '\0') return 0;
    parsed = strtoul(token, &end, 10);
    if (*end != '\0' || errno == ERANGE || parsed > MOVE_LIMIT) return 0;
    *value = (unsigned)parsed;
    return 1;
}

int load_game(Game *g, const char *path) {
    FILE *file = fopen(path, "r"); if (!file) return 0;
    Game candidate; memset(&candidate, 0, sizeof(candidate));
    char header[128];
    if (!fgets(header, sizeof(header), file)) { fclose(file); return 0; }
    char *tokens[7];
    int token_count = 0;
    char *token = strtok(header, " \t\r\n");
    while (token != NULL) {
        if (token_count == 7) { fclose(file); return 0; }
        tokens[token_count++] = token;
        token = strtok(NULL, " \t\r\n");
    }
    if (token_count != 5 && token_count != 6) { fclose(file); return 0; }
    if (!parse_int_token(tokens[0], &candidate.player.row) || !parse_int_token(tokens[1], &candidate.player.col) || !parse_int_token(tokens[2], &candidate.bakery.row) || !parse_int_token(tokens[3], &candidate.bakery.col) || !parse_int_token(tokens[4], &candidate.delivered) || (token_count == 6 && !parse_moves_token(tokens[5], &candidate.moves))) { fclose(file); return 0; }
    int bakery_count = 0;
    for (int r = 0; r < H; r++) {
        if (!fgets(candidate.map[r], W + 2, file) || strlen(candidate.map[r]) != W + 1 || candidate.map[r][W] != '\n') { fclose(file); return 0; }
        candidate.map[r][W] = '\0';
        for (int c = 0; c < W; c++) { char tile = candidate.map[r][c]; if (tile != '#' && tile != ' ' && tile != 'B') { fclose(file); return 0; } if (tile == 'B') bakery_count++; }
    }
    fclose(file);
    if (bakery_count != 1 || !can_step(&candidate, candidate.player) || !can_step(&candidate, candidate.bakery) || candidate.map[candidate.bakery.row][candidate.bakery.col] != 'B' || shortest_path(&candidate) < 0) return 0;
    if (candidate.delivered != 0 && candidate.delivered != 1) return 0;
    if (candidate.moves > 1000000U) return 0;
    if (candidate.delivered != (candidate.player.row == candidate.bakery.row && candidate.player.col == candidate.bakery.col)) return 0;
    *g = candidate; return 1;
}

#ifndef BAKERY_TEST
int main(int argc, char **argv) {
    Game game; char command[32]; unsigned seed = 7;
    if (argc > 2) { fputs("Usage: bakery [seed]\n", stderr); return 2; }
    if (argc > 1) { char *end; unsigned long parsed; errno = 0; parsed = strtoul(argv[1], &end, 10); if (errno || end == argv[1] || *end != '\0' || parsed > 0xffffffffUL) { fprintf(stderr, "Seed must be a whole number.\n"); return 2; } seed = (unsigned)parsed; }
    game_init_seed(&game, seed);
    puts("BREADTH-FIRST BAKERY — 2020 SOURDOUGH DELIVERY");
    printf("Seed %u. Reach B with w/a/s/d. h=distance hint, n=next direction, u=undo, p=put in pantry, l=load pantry, r=restart, q=quit.\n", seed);
    while (!game.delivered) {
        draw(&game); printf("Command [w/a/s/d, h, n, u, r, q]: ");
        if (!fgets(command, sizeof(command), stdin)) break;
        if (command[0] == 'q') break;
        if (command[0] == 'h') { int path = shortest_path(&game); printf(path >= 0 ? "Fresh bread is %d steps away.\n" : "The bakery is unreachable.\n", path); continue; }
        if (command[0] == 'n') { char direction = next_direction(&game); if (direction == '!') puts("The delivery is already complete."); else if (direction == '?') puts("The bakery is unreachable."); else printf("Next direction: %c.\n", direction); continue; }
        if (command[0] == 'u') { puts(undo_move(&game) ? "Undid the last move." : "Nothing to undo."); continue; }
        if (command[0] == 'p') { puts(save_game(&game, "bakery.save") ? "Pantry saved." : "The pantry door is jammed."); continue; }
        if (command[0] == 'l') { puts(load_game(&game, "bakery.save") ? "Pantry loaded." : "No usable pantry save found."); continue; }
        if (command[0] == 'r') { game_init_seed(&game, seed); puts("Bakery restarted with the same seeded layout.\n"); continue; }
        if (!move_player(&game, command[0])) puts("That aisle is blocked. The yeast says no.\n");
    }
    if (game.delivered) { draw(&game); printf("DELIVERY COMPLETE. The loaf survived 2020 in %u moves.\n", game.moves); }
    else puts("Bakery closed. Your sourdough remains emotionally available.");
    return 0;
}
#endif
