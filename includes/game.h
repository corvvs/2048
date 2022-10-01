#ifndef GAME_H
#define GAME_H

#include "types.h"

void init_game(t_game *game, unsigned int random_seed, size_t game_width, size_t game_height);
bool is_game_attribute_consistent(t_game *game);

#endif
