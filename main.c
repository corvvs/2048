#include <ncurses.h>
#include <stdbool.h>

// エラーハンドリング
void init_ncurses()
{
	initscr();
	timeout(-1);
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
}

int main()
{
	init_ncurses();
	while (true) {
		int c = getch();
		flushinp();
		printw("[%d]", c);
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

// #include "board.h"
// #include "debug.h"
// #include "game.h"
// #include "mechanics.h"
// #include <unistd.h>

// int main()
// {
// 	t_game game;
// 	init_game(&game, 1000007, 5, 5);
// 	debug_print_board(&game.current_board);
// 	spawn_a_block(&game.current_board);
// 	for (int i = 1;; ++i) {
// 		printf("[%d-th hand]\n", i);
// 		spawn_a_block(&game.current_board);
// 		debug_print_game(&game);
// 		if (!game.has_won && is_in_winning(&game)) {
// 			printf("[GREAT!!]\n");
// 			break;
// 		}
// 		game.has_won = true;
// 		project_next_states(&game);
// 		if (!is_slidable(&game)) {
// 			printf("[you lost; no available movement]\n");
// 			break;
// 		}

// 		e_move_direction dirs[] = {MD_UP, MD_RIGHT, MD_DOWN, MD_LEFT};
// 		e_move_direction dir;
// 		while (true) {
// 			char m;
// 			read(STDIN_FILENO, &m, 1);
// 			int d = m - '0';
// 			if (0 <= d && d < 4) {
// 				dir = dirs[d];
// 				if (game.movement_results[dir].is_slidable) {
// 					break;
// 				}
// 			}
// 		}
// 		printf("selected move: %d\n", dir);
// 		advance_turn(&game, dir);
// 		debug_print_board(&game.current_board);
// 	}
// }
