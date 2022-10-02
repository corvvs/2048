#include <ncurses.h>
#include <stdbool.h>

#include "board.h"
#include "game.h"
#include "mechanics.h"
#include "refresh_screen.h"
#include "time.h"
#include "types.h"

#define MY_KEY_EOT 4
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
		refresh_screen(g, w);
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

static int ask_yn()
{
	int c;
	while (true) {
		c = getch();
		flushinp();
		switch (c) {
		case 'y':
		case 'n':
			return c;
		default:
			continue;
		}
	}
}

static bool ask_for_exit()
{
	printw("YOU WIN! continue ? y/n\n");
	return ask_yn() == 'n';
}

static bool winning_reaction(t_game *g, WINDOW *w)
{
	if (!is_in_winning(g)) {
		return false;
	}
	refresh_screen(g, w);
	// かち
	// TODO: 勝った時の処理
	// ↓続行を選んだ場合はフラグをセットして続行
	g->has_won = true;
	return ask_for_exit();
}

static bool losing_reaction(t_game *g, WINDOW *w)
{
	if (is_slidable(g)) {
		// 敗北判定
		return false;
	}
	// まけ
	refresh_screen(g, w);
	// TODO: 負けた時の処理
	return true;
}

static void game_loop(t_game *g, WINDOW *w)
{
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

#include <limits.h>
#include <unistd.h>

#include "image.h"
#include "parse_to_image.h"
#include "print_result.h"

int main()
{
	t_game  g;
	WINDOW *w = init_ncurses();
	init_game(&g, time(NULL), 4, 4);
	srand(g.random_seed);
	game_loop(&g, w);
	t_image image = create_result_image(&g.current_board, w);
	endwin();
	print_result(&image, g.score);
}
