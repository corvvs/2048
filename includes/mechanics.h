#ifndef MECHANICS_H
#define MECHANICS_H

#include "types.h"

void debug_print_board(const t_board *b);
void debug_print_result(const t_movement_result *r);
void debug_print_game(const t_game *game);
// 現在の状態から4通りの移動をした結果を計算し, 結果を控えておく
void project_movements(t_game *game);
void progress(t_game *game, e_move_direction direction);
bool is_movable(const t_game *game);

#endif
