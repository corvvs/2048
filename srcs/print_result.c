#include <stdio.h>
#include <unistd.h>

#include "board.h"
#include "ft_utils.h"
#include "image.h"
#include "parse_to_image.h"
#include "types.h"

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

static void print_block_row(const t_block_image_row *block_row, const t_image_size *size)
{
	for (int i = 0; i < size->block_width; i++) {
		wchar_t c = (*block_row)[i];
		if (c) {
			ft_putwchar_fd(1, c);
		} else {
			ft_putwchar_fd(1, ' ');
		}
	}
}

static void print_board_row(const t_board_image_row *board_row, const t_image_size *size)
{
	for (int i = 0; i < size->block_height; i++) {
		for (int j = 0; j < size->board_width; j++) {
			ft_putwchar_fd(1, L'┃');
			print_block_row(&(*board_row)[j].field[i], size);
		}
		ft_putwchar_fd(1, L'┃');
		ft_putwchar_fd(1, L'\n');
	}
}

static void print_delim_line(wchar_t delim[4], const t_image_size *size)
{
	ft_putwchar_fd(1, delim[0]);
	for (int i = 0; i < size->board_width; i++) {
		if (i != 0)
			ft_putwchar_fd(1, delim[2]);
		for (int j = 0; j < size->block_width; j++) {
			ft_putwchar_fd(1, delim[1]);
		}
	}
	ft_putwchar_fd(1, delim[3]);
	ft_putwchar_fd(1, L'\n');
}

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

void print_result(const t_image *img, score_type score)
{
	print_score(score);
	print_image(img);
}
