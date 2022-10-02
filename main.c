#include <ncurses.h>
#include <stdbool.h>
#include <time.h>

#include "board.h"
#include "game.h"
#include "image.h"
#include "mechanics.h"
#include "parse_to_image.h"
#include "print_result.h"
#include "refresh_screen.h"
#include "types.h"

#define MY_KEY_EOT 4
#define MY_KEY_ESC 27

#include <unistd.h>

static void wait_any_key_input(t_game *g, WINDOW *w)
{
	while (true) {
		refresh_screen2(g, w);
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
	g.current_board.field[0][0].score = 2048;
	wait_any_key_input(&g, w);
}

void create_colors()
{
	int offset = 15;
	for (int i = 1; i <= 11; i++) {
		int r = i * (255 / 11);
		init_color(offset + i, r, 0, 0);
		init_pair(i, COLOR_WHITE, offset + i);
		// attrset(COLOR_PAIR(i) | A_BOLD);
		// printw("[r %d i %d]     0000000000    \n", r, i);
	}
}

WINDOW *init_ncurses()
{
	WINDOW *w = initscr();
	timeout(-1);
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	create_colors();
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

static int ask_continue(t_game *g, WINDOW *w)
{
	refresh_screen2(g, w);
	attrset(0 | A_UNDERLINE | A_BOLD);
	printw("YOU WIN! continue ? y/n\n");
	attrset(0 | A_BOLD);
	while (true) {
		int c = getch();
		flushinp();
		switch (c) {
		case KEY_RESIZE:
			refresh_screen2(g, w);
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
	// TODO: 勝った時の処理
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
	refresh_screen(g, w);
	// TODO: 負けた時の処理
	return true;
}

static void game_loop(t_game *g)
{
	WINDOW *w = init_ncurses();
	if (w == NULL) {
		return;
	}
	print_menu(w);
	srand(g->random_seed);
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
	t_image image = create_result_image(&g->current_board, w);
	endwin();
	print_result(&image, g->score);
}

#include <limits.h>
#include <unistd.h>

static size_t get_game_height(int argc, char **argv)
{
	if (argc < 2) {
		return 4;
	}
	char *s = argv[1];
	if ('1' <= s[0] && s[0] <= '5' && s[1] == '\0') {
		return s[0] - '0';
	}
	return 4;
}

static size_t get_game_width(int argc, char **argv)
{
	if (argc < 3) {
		return 4;
	}
	char *s = argv[2];
	if ('1' <= s[0] && s[0] <= '5' && s[1] == '\0') {
		return s[0] - '0';
	}
	return 4;
}

int main(int argc, char **argv)
{
	t_game g;
	size_t height = get_game_height(argc, argv);
	size_t width  = get_game_width(argc, argv);
	init_game(&g, time(NULL), height, width);
	game_loop(&g);
}
