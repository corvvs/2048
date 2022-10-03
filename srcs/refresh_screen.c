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

static void print_score(const t_game *game)
{
	attrset(0 | A_UNDERLINE | A_BOLD);
#ifdef BONUS
	printw("high : %ld ", game->high_score);
#endif
	printw("score : %ld\n", game->score);
	attrset(0);
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

#ifndef BONUS
#define VERTICAL_DELIM L'|'
#else
#define VERTICAL_DELIM L'┃'
#endif
static void print_board_row(const t_board_image_row *board_row, const t_image_size *size)
{
	for (int i = 0; i < size->block_height; i++) {
		for (int j = 0; j < size->board_width; j++) {
			printw("%lc", VERTICAL_DELIM);
			t_color_pair_id color = (*board_row)[j].color;
			attrset(COLOR_PAIR(color) | A_BOLD);
			print_block_row(&(*board_row)[j].field[i], size);
			attrset(0);
		}
		printw("%lc\n", VERTICAL_DELIM);
	}
}

static void print_delim_line(wchar_t delim[4], const t_image_size *size)
{
	printw("%lc", delim[0]);
	for (int i = 0; i < size->board_width; i++) {
		if (i != 0)
			printw("%lc", delim[2]);
		for (int j = 0; j < size->block_width; j++) {
			printw("%lc", delim[1]);
		}
	}
	printw("%lc\n", delim[3]);
}

#ifndef BONUS
static void print_image(const t_image *image)
{
	print_delim_line(L"----", &image->size);
	for (int i = 0; i < image->size.board_height; i++) {
		if (i != 0)
			print_delim_line(L"----", &image->size);
		print_board_row(&image->board[i], &image->size);
	}
	print_delim_line(L"----", &image->size);
}
#else
static void print_image(const t_image *image)
{
	print_delim_line(L"┏━┳┓", &image->size);
	for (int i = 0; i < image->size.board_height; i++) {
		if (i != 0)
			print_delim_line(L"┣━╋┫", &image->size);
		print_board_row(&image->board[i], &image->size);
	}
	print_delim_line(L"┗━┻┛", &image->size);
}
#endif

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
	// printw("l : [%d]\n", line_length);
	clear();
	print_score(game);
	print_image(&image);
}

void refresh_screen_with_key_info(const t_game *game, WINDOW *w)
{
	refresh_screen(game, w);
	print_key_instructions();
}
