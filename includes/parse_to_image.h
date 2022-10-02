#ifndef PARSE_TO_IMAGE_H
#define PARSE_TO_IMAGE_H

#include <ncurses.h>

#include "image.h"
#include "types.h"

void parse_board_to_image(const t_board *board, t_image *image, WINDOW *w);

#endif
