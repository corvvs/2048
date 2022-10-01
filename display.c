#include <ncurses.h>
#include <wchar.h>

#include "types.h"

void print_score(score_type score, int len)
{
	(void)len;
	printw("score : %d\n", score);
}

void print_delim_line(wchar_t delim, int width)
{
	for (int i = 0; i < width; i++) {
		printw("%lc", delim);
	}
	printw("\n");
}

void print_board(const t_board *board, int len)
{
	int delim_count = MAX_GAME_SIZE + 1;
	int block_width = (len - delim_count) / MAX_GAME_SIZE; //小さいときhandle
	int height      = (block_width * MAX_GAME_SIZE + delim_count);
	int width       = height * 2;
	printw("len %d\nbw  %d\nh   %d\nw   %d\n", len, block_width, height, width);
	for (int k = 0; k < MAX_GAME_SIZE; k++) {
		print_delim_line('-', width);
		for (int i = 0; i < block_width; i++) {
			printw("| ");
			for (int j = 0; j < MAX_GAME_SIZE; j++) {
				for (int i = 0; i < block_width * 2; i++) {
					printw(" ");
				}
				printw(" |");
			}
			printw("\n");
		}
	}
	print_delim_line('-', width);
	(void)board;
}

int min_int(int a, int b)
{
	return a < b ? a : b;
}

int get_win_length(WINDOW *w)
{
	int height, width;

	getmaxyx(w, height, width); // errorハンドリング?
	printw("x:[%d] y: [%d]\n", width, height);
	return min_int(width / 2, height - 10);
}

void refresh_screen(const t_game *game, WINDOW *w)
{
	clear();
	int len = get_win_length(w);
	print_score(game->score, len);
	print_board(&game->current_board, len);
}
