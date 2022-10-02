#include <stdio.h>
#include <unistd.h>

#include "board.h"
#include "ft_utils.h"
#include "image.h"
#include "parse_to_image.h"
#include "types.h"

#ifdef __linux__
#define UINT64_FMT "lu"
#else
#define UINT64_FMT "llu"
#endif

static size_t ft_strlen(const char *str)
{
	size_t n = 0;
	while (str[n]) {
		n += 1;
	}
	return n;
}

static void ft_putstr(const char *str)
{
	write(STDOUT_FILENO, str, ft_strlen(str));
}

static void ft_putwchar(wchar_t wc)
{
	char c = wc;
	write(STDOUT_FILENO, &c, 1);
}

static void print_score(score_type score)
{
	ft_putstr("score : ");
	{
		char n[100];
		int  len = ft_utoa_len(score, n, 0);
		write(STDOUT_FILENO, n, len);
	}
	ft_putstr("\n");
}

static void print_delim_line(wchar_t delim, int width)
{
	for (int i = 0; i < width; i++) {
		ft_putwchar(delim);
	}
	ft_putstr("\n");
}

static void print_block_row(const t_block_image_row *block_row, const t_image_size *size)
{
	for (int i = 0; i < size->block_width; i++) {
		wchar_t c = (*block_row)[i];
		if (c) {
			ft_putwchar(c);
		} else {
			ft_putwchar(' ');
		}
	}
}

static void print_board_row(const t_board_image_row *board_row, const t_image_size *size)
{
	for (int i = 0; i < size->block_height; i++) {
		for (int j = 0; j < size->board_width; j++) {
			ft_putstr("|");
			print_block_row(&(*board_row)[j].field[i], size);
		}
		ft_putstr("|");
		ft_putstr("\n");
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
