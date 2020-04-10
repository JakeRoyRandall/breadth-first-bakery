/* Breadth-First Bakery / core snapshot. Fictional 2020-inspired terminal game. */
#include <stdio.h>
#include <string.h>

#define H 9
#define W 15
typedef struct { int row, col; } Point;
typedef struct { char map[H][W + 1]; Point player, bakery; int delivered; } Game;

static const char *core_map[H] = {
    "###############", "#@           B#", "# ### ### ### #",
    "#             #", "# ### ### ### #", "#             #",
    "# ### ### ### #", "#             #", "###############"
};

void game_init(Game *g) {
    memset(g, 0, sizeof(*g));
    for (int r = 0; r < H; r++) strcpy(g->map[r], core_map[r]);
    for (int r = 0; r < H; r++) for (int c = 0; c < W; c++) {
        if (g->map[r][c] == '@') { g->player = (Point){r, c}; g->map[r][c] = ' '; }
        if (g->map[r][c] == 'B') g->bakery = (Point){r, c};
    }
}

int can_step(const Game *g, Point p) { return p.row > 0 && p.row < H - 1 && p.col > 0 && p.col < W - 1 && g->map[p.row][p.col] != '#'; }

int move_player(Game *g, char command) {
    Point next = g->player;
    if (command == 'w') next.row--; else if (command == 's') next.row++;
    else if (command == 'a') next.col--; else if (command == 'd') next.col++;
    else return 0;
    if (!can_step(g, next)) return 0;
    g->player = next;
    if (next.row == g->bakery.row && next.col == g->bakery.col) g->delivered = 1;
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

void draw(const Game *g) {
    for (int r = 0; r < H; r++) { for (int c = 0; c < W; c++) { char tile = g->map[r][c] == '@' ? ' ' : g->map[r][c]; putchar((r == g->player.row && c == g->player.col) ? '@' : tile); } putchar('\n'); }
}

#ifndef BAKERY_TEST
int main(void) {
    Game game; char command[32]; game_init(&game);
    puts("BREADTH-FIRST BAKERY — 2020 SOURDOUGH DELIVERY");
    puts("You are @. Reach B with w/a/s/d. Type h for a path hint, r to reset, q to quit.");
    while (!game.delivered) {
        draw(&game); printf("Command [w/a/s/d, h, r, q]: ");
        if (!fgets(command, sizeof(command), stdin)) break;
        if (command[0] == 'q') break;
        if (command[0] == 'h') { int path = shortest_path(&game); printf(path >= 0 ? "Fresh bread is %d steps away.\n" : "The bakery is unreachable.\n", path); continue; }
        if (command[0] == 'r') { game_init(&game); puts("The dough remembers nothing.\n"); continue; }
        if (!move_player(&game, command[0])) puts("That aisle is blocked. The yeast says no.\n");
    }
    if (game.delivered) { draw(&game); puts("DELIVERY COMPLETE. The loaf survived 2020."); }
    else puts("Bakery closed. Your sourdough remains emotionally available.");
    return 0;
}
#endif
