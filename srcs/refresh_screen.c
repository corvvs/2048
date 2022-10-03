#include <ncurses.h>
#include <wchar.h>

#include "image.h"
#include "parse_to_image.h"
#include "types.h"

static void print_key_instructions()
{
	attrset(0 | A_UNDERLINE | A_BOLD);
	printw("\n[INSTRUCTIONS] Arrow: slide blocks | Esc: exit\n");
	attrset(0);
}

static void print_score(const t_game *game, int width)
{
	(void)width;
	attrset(0 | A_UNDERLINE | A_BOLD);
#ifdef BONUS
	printw("high : %ld ", game->high_score);
#endif
	printw("score : %ld\n", game->score);
	attrset(0);
}

static void print_delim_line(wchar_t delim, int width)
{
	for (int i = 0; i < width; i++) {
		printw("%lc", delim);
	}
	printw("\n");
}

static void print_block_row(const t_block_image_row *block_row, const t_image_size *size)
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

static void print_board_row(const t_board_image_row *board_row, const t_image_size *size)
{
	for (int i = 0; i < size->block_height; i++) {
		for (int j = 0; j < size->board_width; j++) {
			printw("|");
			t_color_pair_id color = (*board_row)[j].color;
			attrset(COLOR_PAIR(color) | A_BOLD);
			print_block_row(&(*board_row)[j].field[i], size);
			attrset(0);
		}
		printw("|");
		printw("\n");
	}
}

static void print_image(const t_image *image, int width)
{
	for (int i = 0; i < image->size.board_height; i++) {
		print_delim_line('-', width);
		print_board_row(&image->board[i], &image->size);
	}
	print_delim_line('-', width);
}

int get_line_length(const t_image_size *size)
{
	int delim_count = size->board_width + 1;
	return size->block_width * size->board_width + delim_count;
}

void refresh_screen(const t_game *game, WINDOW *w)
{
	t_image        image = {};
	const t_board *board = &game->current_board;
	parse_board_to_image(board, &image, w);
	int line_length = get_line_length(&image.size);
	// printw("l : [%d]\n", line_length);
	clear();
	print_score(game, line_length);
	print_image(&image, line_length);
}

void refresh_screen_with_key_info(const t_game *game, WINDOW *w)
{
	refresh_screen(game, w);
	print_key_instructions();
}