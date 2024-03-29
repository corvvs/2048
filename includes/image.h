#ifndef IMAGE_H
#define IMAGE_H
#include "types.h"

#define DELIM_COUNT MAX_GAME_SIZE + 1
#define MAX_BLOCK_IMAGE_SIZE 200
#define MAX_DISPLAY_SIZE MAX_BLOCK_IMAGE_SIZE *MAX_GAME_SIZE

#define COLOR_VARIATION 6

typedef int t_color_pair_id;

typedef wchar_t           t_block_image_row[MAX_BLOCK_IMAGE_SIZE];
typedef t_block_image_row t_block_image_field[MAX_BLOCK_IMAGE_SIZE];

typedef struct s_block_image {
	t_color_pair_id     color;
	t_block_image_field field;
} t_block_image;

typedef t_block_image     t_board_image_row[MAX_GAME_SIZE];
typedef t_board_image_row t_board_image[MAX_GAME_SIZE];

typedef struct {
	int board_width;
	int board_height;
	int block_width;
	int block_height;
} t_image_size;

typedef struct {
	t_board_image board;
	t_image_size  size;
} t_image;

#endif
