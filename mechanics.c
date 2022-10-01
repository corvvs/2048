#include "block.h"
#include "board.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>

static bool move_row_left(size_t width, t_block_row *row)
{
	unsigned int move_to   = 0;
	bool         has_moved = false;
	for (size_t j = 0; j < width; ++j) {
		if ((*row)[j].score == VACANT_BLOCK) {
			continue;
		}
		t_block *b = &(*row)[j];
		if (move_to < j) {
			// ブロック b は動かせる
			// ブロック b を move_to まで移動する
			move_block(b, &(*row)[move_to]);
			has_moved = true;
		}
		b = &(*row)[move_to];
		// b の手前にマスはあるか？
		if (move_to == 0) {
			move_to += 1;
			continue;
		}
		t_block   *remaining    = &(*row)[move_to - 1];
		const bool is_unifiable = (remaining->score == b->score && !remaining->has_unified);
		if (!is_unifiable) {
			move_to += 1;
			continue;
		}
		unify_block(remaining, b);
		has_moved = true;
	}
	return has_moved;
}

static bool move_left(t_movement_result *result)
{
	bool has_moved = false;
	for (size_t i = 0; i < result->board.board_height; ++i) {
		const bool row_has_moved =
			move_row_left(result->board.board_width, &(result->board.field[i]));
		has_moved = has_moved || row_has_moved;
	}
	return has_moved;
}

static void clear_unified_flags(t_board *board)
{
	for (size_t i = 0; i < board->board_height; ++i) {
		for (size_t j = 0; j < board->board_width; ++j) {
			board->field[i][j].has_unified = false;
		}
	}
}

static void
project_movement(const t_board *current, e_move_direction direction, t_movement_result *result)
{
	*result = (t_movement_result){
		.is_movable = false,
		.board      = *current,
	};
	// 合体フラグを全てクリア
	clear_unified_flags(&result->board);
	// 盤面を左に移動する向きに回す
	rotate_to_cannonical(&result->board, direction);
	// 左移動として移動結果を算出
	result->is_movable = move_left(result);
	// 盤面を元の向きに戻す
	rotate_back_from_cannonical(&result->board, direction);
}

void project_movements(t_game *game)
{
	e_move_direction dirs[]        = {MD_UP, MD_RIGHT, MD_DOWN, MD_LEFT};
	const t_board   *current_board = &game->current_board;
	for (unsigned int i = 0; i < ARRAY_LEN(dirs, e_move_direction); ++i) {
		e_move_direction dir = dirs[i];
		project_movement(current_board, dir, &game->movement_results[dir]);
	}
}

static score_type score_increment(const t_board *board)
{
	score_type increment = 0;
	for (size_t i = 0; i < board->board_height; ++i) {
		for (size_t j = 0; j < board->board_width; ++j) {
			const t_block *block = &(board->field[i][j]);
			if (!block->has_unified) {
				continue;
			}
			increment += block->score;
		}
	}
	return increment;
}

// 移動予測から1つ選んで, それを現在の状態とする.
void progress(t_game *game, e_move_direction direction)
{
	t_movement_result *selected = &game->movement_results[direction];
	game->current_board         = selected->board;
	game->score += score_increment(&selected->board);
}

bool is_movable(const t_game *game)
{
	return game->movement_results[MD_UP].is_movable ||
		   game->movement_results[MD_RIGHT].is_movable ||
		   game->movement_results[MD_DOWN].is_movable || game->movement_results[MD_LEFT].is_movable;
}
