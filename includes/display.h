#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>

#include "types.h"

void refresh_screen(const t_game *game, WINDOW *w);

#endif
