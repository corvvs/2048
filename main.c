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
