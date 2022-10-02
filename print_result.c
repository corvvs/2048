#include <stdio.h>

#include "board.h"
#include "image.h"
#include "parse_to_image.h"
#include "types.h"
#include "ft_utils.h"

#ifdef __linux__
#define UINT64_FMT "lu"
#else
#define UINT64_FMT "llu"
#endif

static void print_score(score_type score)
{
	printf("score : %" UINT64_FMT "\n", score);
}

static void print_delim_line(wchar_t delim, int width)
{
	for (int i = 0; i < width; i++) {
		printf("%lc", delim);
	}
	printf("\n");
}

static void print_block_row(const t_block_image_row *block_row, const t_image_size *size)
{
	for (int i = 0; i < size->block_width; i++) {
		wchar_t c = (*block_row)[i];
		if (c) {
			printf("%lc", c);
		} else {
			printf("%lc", ' ');
		}
	}
}

static void print_board_row(const t_board_image_row *board_row, const t_image_size *size)
{
	for (int i = 0; i < size->block_height; i++) {
		for (int j = 0; j < size->board_width; j++) {
			printf("|");
			print_block_row(&(*board_row)[j].field[i], size);
		}
		printf("|");
		printf("\n");
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

void print_result(const t_image *img, score_type score)
{
	int line_length = get_line_length(&img->size);
	print_score(score);
	print_image(img, line_length);
}
