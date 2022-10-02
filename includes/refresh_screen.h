#ifndef REFRESH_SCREEN_H
#define REFRESH_SCREEN_H

#include <ncurses.h>

#include "image.h"
#include "types.h"

void refresh_screen(const t_game *game, WINDOW *w);
void refresh_screen2(const t_game *game, WINDOW *w);
#endif
