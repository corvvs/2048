#include <ncurses.h>
#include <stdbool.h>

#include "board.h"
#include "game.h"
#include "mechanics.h"
#include "refresh_screen.h"
#include "types.h"

#define MY_KEY_ESC 27

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

static e_slide_direction key_to_direction(int direction_key)
{
	switch (direction_key) {
	case KEY_UP:
		return MD_UP;
	case KEY_RIGHT:
		return MD_RIGHT;
	case KEY_DOWN:
		return MD_DOWN;
	case KEY_LEFT:
		return MD_LEFT;
	}
	// 到達しないはず
	return 0;
}

static int get_keych()
{
	int c;
	while (true) {
		c = getch();
		flushinp();
		switch (c) {
		case KEY_RESIZE:
		case KEY_UP:
		case KEY_DOWN:
		case KEY_LEFT:
		case KEY_RIGHT:
		case MY_KEY_ESC:
		case EOF:
			break;
		default:
			continue;
		}
		break;
	}
	return c;
}

static bool key_reaction(t_game *g, WINDOW *w)
{
	while (true) {
		int c = get_keych();
		switch (c) {
		case MY_KEY_ESC:
			break;
		case EOF:
			break;
		case KEY_RESIZE:
			refresh_screen(g, w);
			continue;
		case KEY_UP:
		case KEY_DOWN:
		case KEY_LEFT:
		case KEY_RIGHT: {
			e_slide_direction dir = key_to_direction(c);
			// とりあえずこの状態でキーを返す関数を切り出す
			if (!g->movement_results[dir].is_slidable) {
				continue;
			}
			advance_turn(g, dir);
			return false;
		}
		}
		return true;
	}
}

#include <limits.h>
int main()
{
	t_game  g;
	WINDOW *w = init_ncurses();
	init_game(&g, 1, 4, 4);
	spawn_a_block(&g.current_board);
	refresh_screen(&g, w);
	while (true) {
		spawn_a_block(&g.current_board);
		project_next_states(&g);
		if (!g.has_won) { // 勝利判定
			if (is_in_winning(&g)) {
				// かち
				break;
			}
		}
		if (!is_slidable(&g)) { // 敗北判定
			// まけ
			break;
		}
		// 描画
		refresh_screen(&g, w);
		// キー入力への反応
		const bool exit_gameloop = key_reaction(&g, w);
		if (exit_gameloop) {
			break;
		}
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

// 		e_slide_direction dirs[] = {MD_UP, MD_RIGHT, MD_DOWN, MD_LEFT};
// 		e_slide_direction dir;
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
