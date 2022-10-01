#include <ncurses.h>
#include <wchar.h>

#include "display.h"
#include "types.h"
#include "util.h"

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

int get_usable_win_width(WINDOW *w)
{
	int height, width;

	getmaxyx(w, height, width); // errorハンドリング?
	printw("x:[%d] y: [%d]\n", width, height);
	int size = min_int(width / 2, height - 10) * 2;
	return min_int(size, MAX_DISPLAY_SIZE);
}

void set_wchar(t_block_image_row *row, wchar_t c, int size)
{
	for (int i = 0; i < size; i++) {
		(*row)[i] = c;
	}
}

void set_payload(t_block_image_row *dest, score_type src_num, int width)
{
	char numstr[64];
	int  digit;
	if (src_num == 0) {
		digit = 0;
	} else {
		digit = ft_utoa_len(src_num, numstr, 0);
	}
	int left_padding_size = width >= digit ? (width - digit) / 2 : 0;
	for (int j = 0; j < digit; j++) {
		(*dest)[left_padding_size + j] = numstr[j];
	}
}

void parse_to_block_image(score_type num, t_block_image *img, const t_image_size *size)
{
	int line_padding_size = size->block_height / 2;

	set_payload(&(*img)[line_padding_size], num, size->block_width);
}

int parse_board_to_image(const t_board *board, t_image *image, int width)
{
	image->size.board_width  = board->board_width;
	image->size.board_height = board->board_height;
	int block_width          = (width - DELIM_COUNT) / board->board_width; //小さいときhandle
	if (block_width <= 0) {                                                // fix
		return -1;
	}
	image->size.block_width  = block_width;
	image->size.block_height = block_width / 2;
	for (unsigned int i = 0; i < board->board_height; i++) {
		for (unsigned int j = 0; j < board->board_width; j++) {
			score_type num = board->field[i][j].score;
			parse_to_block_image(num, &image->board[i][j], &image->size);
		}
	}
	return 0;
}

void refresh_screen(const t_game *game, WINDOW *w)
{
	t_image        image = {};
	int            width = get_usable_win_width(w);
	const t_board *board = &game->current_board;

	parse_board_to_image(board, &image, width);
	int line_length = image.size.block_width * board->board_width + DELIM_COUNT;
	clear();
	print_score(game->score, line_length);
	print_image(&image, line_length);
}
