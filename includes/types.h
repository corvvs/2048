#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>
#include <stdbool.h>

// ブロック
typedef struct s_block {
	// ブロックの持っている数(スコア)
	unsigned long long	score;
	// ブロックの縦位置
	unsigned int		row;
	// ブロックの横位置
	unsigned int		col;
	// 合体フラグ
	bool				has_united;
}	t_block;

#define MAX_GAME_SIZE 4
typedef t_block	t_board[MAX_GAME_SIZE][MAX_GAME_SIZE];

// 移動結果
typedef struct s_movement_result {
	// 移動が可能かどうか
	bool 	is_movable;
	// 移動した結果
	// (is_movable == false の時の内容には意味がない)
	t_board	result;
}	t_movement_result;

// 移動方向
enum e_move_direction {
	MD_UP		= 0,
	MD_RIGHT	= 1,
	MD_DOWN		= 2,
	MD_LEFT		= 3,
};

// ゲームのパラメータ
// ゲーム開始時に設定され, 以後固定される
typedef struct s_game_attribute {
	size_t				board_width;
	size_t				board_height;
}	t_game_attribute;

// ゲームの状態
// ゲーム開始時に初期化されるが, その後も変化する.
typedef struct t_game_state {
	//　盤面
	t_board				current_board;
	// 現在の状態で移動(上下左右4方向)操作をした時の、移動可否と結果
	t_movement_result	movement_results[4];
	// 現在のスコア
	unsigned long long	score;
}	t_game_state;

typedef struct s_game {
	t_game_attribute	attribute;
	t_game_state		state;
}	t_game;

#endif
