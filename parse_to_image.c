#include <ncurses.h>
#include <wchar.h>

#include "image.h"
#include "util.h"

#define INFOMATION_SIZE 5
static int get_usable_win_width(WINDOW *w)
{
	int height, width;

	getmaxyx(w, height, width); // errorハンドリング?
	int usable_height = max_int(0, height - INFOMATION_SIZE);
	int usable_width  = min_int(width / 2, usable_height) * 2;
	// printw("x : [%d] y : [%d]\n", width, height);
	return min_int(usable_width, MAX_DISPLAY_SIZE);
}

static void set_payload(t_block_image_row *dest, score_type src_num, int width)
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

static void parse_to_block_image(score_type num, t_block_image *img, const t_image_size *size)
{
	int line_padding_size = size->block_height / 2;

	set_payload(&(*img)[line_padding_size], num, size->block_width);
}

void parse_board_to_image(const t_board *board, t_image *image, WINDOW *w)
{
	int width                = get_usable_win_width(w);
	int delim_count          = board->board_width + 1;
	image->size.board_width  = board->board_width;
	image->size.board_height = board->board_height;
	image->size.block_width  = max_int(0, (width - delim_count) / board->board_width);
	image->size.block_height = max_int(1, image->size.block_width / 2);
	// printw("bw: [%d] bh: [%d]\n", image->size.block_width, image->size.block_height);
	for (unsigned int i = 0; i < board->board_height; i++) {
		for (unsigned int j = 0; j < board->board_width; j++) {
			score_type num = board->field[i][j].score;
			parse_to_block_image(num, &image->board[i][j], &image->size);
		}
	}
}

t_image create_result_image(const t_board *board, WINDOW *w)
{
	t_image image;
	parse_board_to_image(board, &image, w);
	return image;
}

// bonusで使うかも
void set_wchar(t_block_image_row *row, wchar_t c, int size)
{
	for (int i = 0; i < size; i++) {
		(*row)[i] = c;
	}
}
