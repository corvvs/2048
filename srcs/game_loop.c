#include <ncurses.h>
#include <stdbool.h>

#include "board.h"
#include "ft_utils.h"
#include "game.h"
#include "image.h"
#include "mechanics.h"
#include "parse_to_image.h"
#include "refresh_screen.h"
#include "types.h"

#define MY_KEY_EOT 4
#define MY_KEY_ESC 27

static void wait_any_key_input(t_game *g, WINDOW *w)
{
	while (true) {
		refresh_screen(g, w);
		attrset(0 | A_UNDERLINE | A_BOLD);
		printw("Press any key to start game !\n");
		if (getch() == KEY_RESIZE) {
			continue;
		}
		break;
	}
	attrset(0 | A_BOLD);
}

void print_menu(WINDOW *w)
{
	t_game g;
	init_game(&g, 1, 1, 1);
	g.current_board.field[0][0].score = WIN_VALUE;
	wait_any_key_input(&g, w);
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
		refresh_screen_with_key_info(g, w);
		int c = get_keych();
		switch (c) {
		case MY_KEY_ESC:
			break;
		case MY_KEY_EOT:
			break;
		case KEY_RESIZE:
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

static int ask_continue(t_game *g, WINDOW *w)
{
	refresh_screen(g, w);
	attrset(0 | A_UNDERLINE | A_BOLD);
	printw("YOU WIN! continue ? y/n\n");
	attrset(0 | A_BOLD);
	while (true) {
		int c = getch();
		flushinp();
		switch (c) {
		case KEY_RESIZE:
			refresh_screen(g, w);
			attrset(0 | A_UNDERLINE | A_BOLD);
			printw("YOU WIN! continue ? y/n\n");
			attrset(0 | A_BOLD);
			continue;
		case 'y':
		case 'n':
			return c;
		default:
			continue;
		}
	}
}

static bool winning_reaction(t_game *g, WINDOW *w)
{
	if (!is_in_winning(g)) {
		return false;
	}
	// かち
	// ↓続行を選んだ場合はフラグをセットして続行
	g->has_won = true;
	return ask_continue(g, w) == 'n';
}

static bool losing_reaction(t_game *g, WINDOW *w)
{
	if (is_slidable(g)) {
		// 敗北判定
		return false;
	}
	// まけ
	refresh_screen_with_key_info(g, w);
	return true;
}

void game_loop(t_game *g, WINDOW *w)
{
	print_menu(w);
	spawn_a_block(&g->current_board);
	while (true) {
		spawn_a_block(&g->current_board);
		project_next_states(g);
		// まだ勝利状態でなければ勝利判定
		if (!g->has_won) {
			const bool exit_gameloop = winning_reaction(g, w);
			if (exit_gameloop) {
				break;
			}
		}
		// 敗北判定
		{
			const bool exit_gameloop = losing_reaction(g, w);
			if (exit_gameloop) {
				break;
			}
		}
		// キー入力への反応
		{
			const bool exit_gameloop = key_reaction(g, w);
			if (exit_gameloop) {
				break;
			}
		}
	}
}
