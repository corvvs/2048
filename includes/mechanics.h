#ifndef MECHANICS_H
#define MECHANICS_H

#include "types.h"

void project_next_states(t_game *game);
void advance_turn(t_game *game, e_slide_direction direction);
bool is_in_winning(const t_game *game);
bool is_slidable(const t_game *game);

#endif
