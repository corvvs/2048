#include "block.h"
#include "types.h"

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
		.has_unified = false,
		.score       = s,
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

void rotate_to_cannonical(t_board *board, e_move_direction direction)
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
void rotate_back_from_cannonical(t_board *board, e_move_direction direction)
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
