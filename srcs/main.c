#include <ncurses.h>
#include <stdbool.h>
#include <time.h>

#include "board.h"
#include "ft_utils.h"
#include "game.h"
#include "image.h"
#include "parse_to_image.h"
#include "print_result.h"
#include "types.h"

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
	if ('1' <= s[0] && s[0] <= ('0' + MAX_GAME_SIZE) && s[1] == '\0') {
		return s[0] - '0';
	}
	return 4;
}

static void create_colors()
{
	const int offset = 15;
	for (int i = 1; i <= COLOR_VARIATION; i++) {
		int r = i * (255 / COLOR_VARIATION);
		init_color(offset + i, r, 0, 0);
		init_pair(i, COLOR_WHITE, offset + i);
		// attrset(COLOR_PAIR(i) | A_BOLD);
		// printw("[r %d i %d]     0000000000    \n", r, i);
	}
}

static WINDOW *init_ncurses()
{
	WINDOW *w = initscr();
	if (w == NULL) {
		return NULL;
	}
	timeout(-1);
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();
	create_colors();
	return w;
}

static void run_game(t_game *g)
{
	WINDOW *w = init_ncurses();
	if (w == NULL) {
		return;
	}
	srand(g->random_seed);
	game_loop(g, w);
	t_image image = create_result_image(&g->current_board, w);
	endwin();
	print_result(&image, g->score);
}
#include <locale.h>
int main(int argc, char **argv)
{
#ifdef BONUS
	setlocale(LC_ALL, "");
#endif
	t_game *g = malloc(sizeof(t_game));
	size_t height = get_game_height(argc, argv);
	size_t width  = get_game_width(argc, argv);
	init_game(g, time(NULL), height, width);
	run_game(g);
}
