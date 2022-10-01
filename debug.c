#include "debug.h"
#include <stdio.h>

#ifdef __linux__
#define UINT64_FMT "lu"
#else
#define UINT64_FMT "llu"
#endif
void debug_print_board(const t_board *b)
{
	printf("[H: %zu, W: %zu]\n", b->board_height, b->board_width);
	for (unsigned int i = 0; i < b->board_height; ++i) {
		for (unsigned int j = 0; j < b->board_width; ++j) {
			if (b->field[i][j].has_unified) {
				printf("[%4" UINT64_FMT "]", b->field[i][j].score);
			} else {
				printf(" %4" UINT64_FMT " ", b->field[i][j].score);
			}
		}
		printf("\n");
	}
}

void debug_print_result(const t_movement_result *r)
{
	printf("movable?: %s\n", r->is_movable ? "Y" : "n");
	debug_print_board(&r->board);
}

void debug_print_game(const t_game *game)
{
	printf("score: %" UINT64_FMT "\n", game->score);
	debug_print_board(&game->current_board);
}
