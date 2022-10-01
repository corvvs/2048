#include "includes/types.h"
#include <stdio.h>
#include <stdlib.h>

void debug_print_board(const t_board *b)
{
	printf("[H: %zu, W: %zu]\n", b->board_height, b->board_width);
	for (unsigned int i = 0; i < b->board_height; ++i) {
		for (unsigned int j = 0; j < b->board_width; ++j) {
			if (b->field[i][j].has_united) {
				printf("[%4llu]", b->field[i][j].score);
			} else {
				printf(" %4llu ", b->field[i][j].score);
			}
		}
		printf("\n");
	}
}

void debug_print_result(const t_movement_result *r)
{
	printf("movable?: %s\n", r->is_movable ? "Y" : "n");
	debug_print_board(&r->board);
}

void debug_print_game(const t_game *game)
{
	printf("score: %llu\n", game->score);
	debug_print_board(&game->current_board);
}

// 空きマスのどこかに1つブロックを置く
void spawn_block(t_board *b)
{
	size_t positions[MAX_GAME_SIZE * MAX_GAME_SIZE] = {};
	size_t vacants                                  = 0;
	for (unsigned int i = 0; i < b->board_height; ++i) {
		for (unsigned int j = 0; j < b->board_width; ++j) {
			if (b->field[i][j].score != VACANT_BLOCK) {
				continue;
			}
			positions[vacants] = i * MAX_GAME_SIZE + j;
			vacants += 1;
		}
	}
	if (vacants == 0) {
		return;
	}
	size_t idx_place = rand() % vacants;
	size_t i_place   = positions[idx_place] / MAX_GAME_SIZE;
	size_t j_place   = positions[idx_place] % MAX_GAME_SIZE;
	// (i_place, j_place) に置く
	score_type s               = rand() % 10 > 0 ? 2 : 4;
	b->field[i_place][j_place] = (t_block){
		.has_united = false,
		.score      = s,
	};
}

static void swap_size_t(size_t *a, size_t *b)
{
	size_t c = *a;
	*a       = *b;
	*b       = c;
}

static void swap_block(t_block *a, t_block *b)
{
	t_block c = *a;
	*a        = *b;
	*b        = c;
}

// ブロック *b を空の状態にする
static void erase_block(t_block *b)
{
	*b = (t_block){
		.has_united = false,
		.score      = VACANT_BLOCK,
	};
}

// ブロック *from を *to に移動させる
static void move_block(t_block *from, t_block *to)
{
	*to = *from;
	erase_block(from);
}

// ブロック *erased を *remaining に合体させ, *erased は削除する
static void unify_block(t_block *remaining, t_block *erased)
{
	remaining->score += erased->score;
	remaining->has_united = true;
	erase_block(erased);
}

// 盤面を左右反転する
static void flip_board(t_board *board)
{
	for (unsigned int i = 0; i < board->board_height; ++i) {
		for (unsigned int j = 0; j < board->board_width - j - 1; ++j) {
			unsigned int k = board->board_width - j - 1;
			swap_block(&board->field[i][j], &board->field[i][k]);
		}
	}
}

// 盤面を転置する
static void transpose_board(t_board *board)
{
	for (unsigned int i = 0; i < MAX_GAME_SIZE; ++i) {
		for (unsigned int j = i + 1; j < MAX_GAME_SIZE; ++j) {
			swap_block(&board->field[i][j], &board->field[j][i]);
		}
	}
	// 転置の場合は縦横サイズも入れ替える
	swap_size_t(&board->board_height, &board->board_width);
}

static void rotate_to_cannonical(t_board *board, e_move_direction direction)
{
	// cannonical: 左に向かって移動する
	// left:  なにもしない
	// right: 左右反転する
	// up:    転置する
	// down:  転置して左右反転する; 順序に注意
	switch (direction) {
	case MD_LEFT:
		break;
	case MD_RIGHT:
		flip_board(board);
		break;
	case MD_UP:
		transpose_board(board);
		break;
	case MD_DOWN:
		transpose_board(board);
		flip_board(board);
		break;
	}
}

// rotate_to_cannonical の逆操作をする.
static void rotate_back_from_cannonical(t_board *board, e_move_direction direction)
{
	// 転置と反転はいずれも自分自身が逆操作になるので,
	// この関数では転置と反転をする順序にだけ気をつければよい. left:  なにもしない right:
	// 左右反転する up:    転置する down:  左右反転して転置する; 順序に注意
	switch (direction) {
	case MD_LEFT:
		break;
	case MD_RIGHT:
		flip_board(board);
		break;
	case MD_UP:
		transpose_board(board);
		break;
	case MD_DOWN:
		flip_board(board);
		transpose_board(board);
		break;
	}
}

static bool move_row_left(size_t width, t_block_row *row)
{
	unsigned int move_to   = 0;
	bool         has_moved = false;
	for (unsigned int j = 0; j < width; ++j) {
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
		const bool is_unifiable = (remaining->score == b->score && !remaining->has_united);
		if (!is_unifiable) {
			move_to += 1;
			continue;
		}
		unify_block(remaining, b);
	}
	return has_moved;
}

static bool move_left(t_movement_result *result)
{
	bool has_moved = false;
	for (unsigned int i = 0; i < result->board.board_height; ++i) {
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
			board->field[i][j].has_united = false;
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
			if (!block->has_united) {
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
