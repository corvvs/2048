#include "block.h"
#include "board.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>

static bool move_row_left(size_t width, t_block_row *row)
{
	bool has_updated = false;
	for (size_t j = 0, move_to = 0; j < width; ++j) {
		t_block *b = &(*row)[j];
		if (b->score == VACANT_BLOCK) {
			continue;
		}
		// 移動
		if (move_to < j) {
			// ブロック b は動かせるので, ブロック b を move_to まで移動する
			// ... のだが, 移動先は空のはずなので, 移動ではなくスワップでよい.
			t_block *dest = &(*row)[move_to];
			swap_block(b, dest);
			has_updated = true;
			b           = dest;
		}
		// 合体
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
		has_updated = true;
	}
	return has_updated;
}

static bool slide_toward_left(t_movement_result *result)
{
	t_board *board       = &result->board;
	bool     has_updated = false;
	for (size_t i = 0; i < board->board_height; ++i) {
		const bool row_has_moved = move_row_left(board->board_width, &(board->field[i]));
		has_updated              = has_updated || row_has_moved;
	}
	return has_updated;
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
project_slide(const t_board *current, e_slide_direction direction, t_movement_result *result)
{
	*result = (t_movement_result){
		.is_slidable = false,
		.board       = *current,
	};
	// 合体フラグを全てクリア
	clear_unified_flags(&result->board);
	// 盤面を左に移動する向きに回す
	rotate_to_canonical(&result->board, direction);
	// 左移動として移動結果を算出
	result->is_slidable = slide_toward_left(result);
	// 盤面を元の向きに戻す
	rotate_back_from_canonical(&result->board, direction);
}

// 現在の状態から4通りの移動をした結果を計算し, 結果を控えておく
void project_next_states(t_game *game)
{
	e_slide_direction dirs[]        = {MD_UP, MD_RIGHT, MD_DOWN, MD_LEFT};
	const t_board    *current_board = &game->current_board;
	for (unsigned int i = 0; i < ARRAY_LEN(dirs, e_slide_direction); ++i) {
		e_slide_direction dir = dirs[i];
		project_slide(current_board, dir, &game->movement_results[dir]);
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

// 移動予測から(可能な)1つ選んで, 次のターンに進む
void advance_turn(t_game *game, e_slide_direction direction)
{
	t_movement_result *selected = &game->movement_results[direction];
	game->current_board         = selected->board;
	game->score += score_increment(&selected->board);
}

static bool is_power_of_2(int n)
{
	return n > 0 && (n & (n - 1)) == 0b0;
}

// 現在勝利状態であるかどうか判定する
bool is_in_winning(const t_game *game)
{
	if (!is_power_of_2(WIN_VALUE)) {
		return false;
	}
	const t_block_array *field = &game->current_board.field;
	for (size_t i = 0; i < game->board_height; ++i) {
		for (size_t j = 0; j < game->board_width; ++j) {
			if ((*field)[i][j].score >= WIN_VALUE) {
				return true;
			}
		}
	}
	return false;
}

// 実行可能な移動があるかどうか判定する
bool is_slidable(const t_game *game)
{
	return game->movement_results[MD_UP].is_slidable ||
		   game->movement_results[MD_RIGHT].is_slidable ||
		   game->movement_results[MD_DOWN].is_slidable ||
		   game->movement_results[MD_LEFT].is_slidable;
}
