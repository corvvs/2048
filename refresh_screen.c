#include <ncurses.h>
#include <wchar.h>

#include "image.h"
#include "parse_to_image.h"
#include "refresh_screen.h"
#include "types.h"

void print_score(score_type score, int width)
{
	(void)width;
	printw("score : %d\n", score);
}

void print_delim_line(wchar_t delim, int width)
{
	for (int i = 0; i < width; i++) {
		printw("%lc", delim);
	}
	printw("\n");
}

void print_block_row(const t_block_image_row *block_row, const t_image_size *size)
{
	for (int i = 0; i < size->block_width; i++) {
		wchar_t c = (*block_row)[i];
		if (c) {
			printw("%lc", c);
		} else {
			printw("%lc", ' ');
		}
	}
}

void print_board_row(const t_board_image_row *board_row, const t_image_size *size)
{
	for (int i = 0; i < size->block_height; i++) {
		for (int j = 0; j < size->board_width; j++) {
			printw("|");
			print_block_row(&(*board_row)[j][i], size);
		}
		printw("|");
		printw("\n");
	}
}

void print_image(const t_image *image, int width)
{
	for (int i = 0; i < image->size.board_height; i++) {
		print_delim_line('-', width);
		print_board_row(&image->board[i], &image->size);
	}
	print_delim_line('-', width);
}

void refresh_screen(const t_game *game, WINDOW *w)
{
	t_image        image = {};
	const t_board *board = &game->current_board;

	parse_board_to_image(board, &image, w);
	int line_length = image.size.block_width * board->board_width + DELIM_COUNT;
	// printw("l : [%d]\n", line_length);
	clear();
	print_score(game->score, line_length);
	print_image(&image, line_length);
}