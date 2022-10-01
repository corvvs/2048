#include <ncurses.h>
#include <stdbool.h>

#include "display.h"
#include "game.h"
#include "types.h"

// エラーハンドリング
WINDOW *init_ncurses()
{
	WINDOW *w = initscr();
	timeout(-1);
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	return w;
}

int main()
{
	t_game  g;
	WINDOW *w = init_ncurses();
	(void)w;
	init_game(&g, 1, 4, 4);
	while (true) {
		int c = getch();
		flushinp();
		if (c == KEY_RESIZE) {
		}
		refresh_screen(&g, w);
		// printw("€\n");
		// printw("[%d]", c);
	}
	endwin();
}

// while (true) {
// 	t_moved moved;

// 	put_new_value(g_board);
// 	print_board(g_board);
// 	create_moved_board_for_every_direction(g_board, &moved);
// 	if (!moved.can_move) {
// 		break;
// 	}
// 	int direction = Getchar(moved.movable_set);
// 	if (direction == EOF || direction == ESC) {
// 		break;
// 	}
// 	g_board = moved.boards[direction];
// 	print_board(g_board);
// 	if (!is_victory(g_board)) {
// 		continue;
// 	}
// 	int res = display_dialogue();
// 	if (res == end) {
// 		break;
// 	}
// }

// #include "includes/game.h"
// #include "includes/mechanics.h"

// int main()
// {
// 	t_game game;
// 	init_game(&game, 1000007, 4, 4);
// 	debug_print_board(&game.current_board);
// 	spawn_block(&game.current_board);
// 	for (int i = 1; i <= 50; ++i) {
// 		printf("[%d-th hand]\n", i);
// 		spawn_block(&game.current_board);
// 		debug_print_board(&game.current_board);
// 		project_movements(&game);
// 		// printf("[move_up]\n");
// 		// debug_print_result(&game.movement_results[MD_UP]);
// 		// printf("[move_right]\n");
// 		// debug_print_result(&game.movement_results[MD_RIGHT]);
// 		// printf("[move_down]\n");
// 		// debug_print_result(&game.movement_results[MD_DOWN]);
// 		// printf("[move_left]\n");
// 		// debug_print_result(&game.movement_results[MD_LEFT]);
// 		if (!is_movable(&game)) {
// 			printf("[you lost; no available movement]\n");
// 			break;
// 		}

// 		e_move_direction dirs[] = {MD_UP, MD_RIGHT, MD_DOWN, MD_LEFT};
// 		e_move_direction dir;
// 		while (true) {
// 			dir = dirs[rand() % 4];
// 			if (game.movement_results[dir].is_movable) {
// 				break;
// 			}
// 		}
// 		printf("selected move: %d\n", dir);
// 		progress(&game, dir);
// 		debug_print_board(&game.current_board);
// 	}
// }
