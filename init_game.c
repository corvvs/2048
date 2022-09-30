#include "includes/types.h"

// game 構造体の初期化
void init_game(t_game *game, unsigned int random_seed, size_t game_width, size_t game_height)
{
	*game = (t_game){
		.random_seed      = random_seed,
		.board_height     = game_height,
		.board_width      = game_width,
		.has_won          = false,
		.has_lose         = false,
		.score            = 0,
		.current_board    = {},
		.movement_results = {},
	};
};

// game 構造体の attribute が適切かどうかをチェックする
bool is_game_attribute_consistent(t_game *game)
{
	if (game->board_height > MAX_GAME_SIZE) {
		return false;
	}
	if (game->board_width > MAX_GAME_SIZE) {
		return false;
	}
	return true;
}
