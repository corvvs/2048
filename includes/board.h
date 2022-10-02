#ifndef BOARD_H
#define BOARD_H

#include "types.h"

void spawn_a_block(t_board *b);
void rotate_to_canonical(t_board *board, e_slide_direction direction);
void rotate_back_from_canonical(t_board *board, e_slide_direction direction);

#endif
