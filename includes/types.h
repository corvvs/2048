#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#define ARRAY_LEN(array, element_type) (sizeof(array) / sizeof(element_type))

#define VACANT_BLOCK 0

typedef uint64_t score_type;

// ブロック
typedef struct s_block {
	// ブロックの持っている数(スコア)
	// 値が VACANT_BLOCK ならそこは空きブロック
	score_type score;
	// 合体フラグ
	bool has_unified;
} t_block;

#define MAX_GAME_SIZE 5
typedef t_block     t_block_row[MAX_GAME_SIZE];
typedef t_block_row t_block_array[MAX_GAME_SIZE];

typedef struct s_board {
	size_t        board_width;
	size_t        board_height;
	t_block_array field;
} t_board;

// 移動結果
typedef struct s_movement_result {
	// 移動が可能かどうか
	bool is_slidable;
	// 移動した結果
	// (is_slidable == false の時の内容には意味がない)
	t_board board;
	// 移動により増えたスコアは,
	// 「board 上の合体フラグがついているブロックの score の総和」
	// で計算できるので, データとして保持しておく必要がない
} t_movement_result;

// 移動方向
typedef enum {
	MD_UP    = 0,
	MD_RIGHT = 1,
	MD_DOWN  = 2,
	MD_LEFT  = 3,
} e_move_direction;

enum e_const {
	WIN_VALUE = 2048,
};

// ゲームのパラメータ
// ゲーム開始時に設定され, 以後固定される
// ゲームの状態
// ゲーム開始時に初期化されるが, その後も変化する.
typedef struct t_game {
	// 乱数シード
	unsigned int random_seed;
	size_t       board_width;
	size_t       board_height;
	// 勝利フラグ
	// 勝利した後でもゲームは続行できる
	bool has_won;
	// 敗北フラグ
	// 敗北した後はゲームは続行できない
	bool has_lose;
	//　盤面
	t_board current_board;
	// 現在の状態で移動(上下左右4方向)操作をした時の、移動可否と結果
	t_movement_result movement_results[4];
	// 現在のスコア
	score_type score;
} t_game;

#endif
