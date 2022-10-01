#ifndef MECHANICS_H
#define MECHANICS_H

#include "types.h"

void project_movements(t_game *game);
void progress(t_game *game, e_move_direction direction);
bool is_movable(const t_game *game);

#endif
