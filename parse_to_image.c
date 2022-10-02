#include <ncurses.h>
#include <wchar.h>

#include "image.h"
#include "util.h"

#define INFOMATION_SIZE 5
int get_usable_win_width(WINDOW *w)
{
	int height, width;

	getmaxyx(w, height, width); // errorハンドリング?
	int usable_height = max_int(0, height - INFOMATION_SIZE);
	int usable_width  = min_int(width / 2 - 1, usable_height) * 2;
	//-1がないと何故か崩れるわけわからん
	// printw("x : [%d] y : [%d]\n", width, height);
	return min_int(usable_width, MAX_DISPLAY_SIZE);
}

void set_wchar(t_block_image_row *row, wchar_t c, int size)
{
	for (int i = 0; i < size; i++) {
		(*row)[i] = c;
	}
}

void set_payload(t_block_image_row *dest, score_type src_num, int width)
{
	if (src_num == VACANT_BLOCK) {
		return;
	}
	char numstr[64];
	int  digit             = ft_utoa_len(src_num, numstr, 0);
	int  left_padding_size = max_int(0, (width - digit) / 2);
	for (int i = 0; i < digit; i++) {
		(*dest)[left_padding_size + i] = numstr[i];
	}
}

void parse_to_block_image(score_type num, t_block_image *img, const t_image_size *size)
{
	int line_padding_size = size->block_height / 2;

	set_payload(&(*img)[line_padding_size], num, size->block_width);
}

void parse_board_to_image(const t_board *board, t_image *image, WINDOW *w)
{
	int width                = get_usable_win_width(w);
	image->size.board_width  = board->board_width;
	image->size.board_height = board->board_height;
	image->size.block_width  = max_int(0, (width - DELIM_COUNT) / board->board_width);
	image->size.block_height = max_int(1, image->size.block_width / 2);
	// printw("bw: [%d] bh: [%d]\n", image->size.block_width, image->size.block_height);
	for (unsigned int i = 0; i < board->board_height; i++) {
		for (unsigned int j = 0; j < board->board_width; j++) {
			score_type num = board->field[i][j].score;
			parse_to_block_image(num, &image->board[i][j], &image->size);
		}
	}
}
