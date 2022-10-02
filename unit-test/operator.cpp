#include "types.h"
#include <iostream>

bool operator==(const t_block &lhs, const t_block &rhs)
{
	return lhs.has_unified == rhs.has_unified && lhs.score == rhs.score;
}

bool operator==(const t_board &lhs, const t_board &rhs)
{
	if (lhs.board_width != rhs.board_width || lhs.board_height != rhs.board_height) {
		return false;
	}
	for (size_t i = 0; i < lhs.board_height; i++) {
		for (size_t j = 0; i < lhs.board_width; i++) {
			if (!(lhs.field[i][j] == rhs.field[i][j])) {
				return false;
			}
		}
	}
	return true;
}

bool operator==(const t_movement_result &lhs, const t_movement_result &rhs)
{
	return lhs.is_slidable == rhs.is_slidable && lhs.board == rhs.board;
}

bool operator==(const t_game &lhs, const t_game &rhs)
{
	return lhs.random_seed == rhs.random_seed && lhs.board_height == rhs.board_height &&
		   lhs.board_width == rhs.board_width && lhs.has_won == rhs.has_won &&
		   lhs.has_lose == rhs.has_lose && lhs.score == rhs.score &&
		   lhs.current_board == rhs.current_board &&
		   lhs.movement_results[0] == rhs.movement_results[0] &&
		   lhs.movement_results[1] == rhs.movement_results[1] &&
		   lhs.movement_results[2] == rhs.movement_results[2] &&
		   lhs.movement_results[3] == rhs.movement_results[3];
}

bool operator!=(const t_block &lhs, const t_block &rhs)
{
	return !(lhs == rhs);
}

bool operator!=(const t_board &lhs, const t_board &rhs)
{
	return !(lhs == rhs);
}

bool operator!=(const t_movement_result &lhs, const t_movement_result &rhs)
{
	return !(lhs == rhs);
}

bool operator!=(const t_game &lhs, const t_game &rhs)
{
	return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &os, const t_block &v)
{
	os << "\nscore: " << v.score << "\nhas_united: " << v.has_unified;
	return os;
}

std::ostream &operator<<(std::ostream &os, const t_board &v)
{
	os << "\nwidth: " << v.board_width << "\nhight: " << v.board_height << "\n";
	for (size_t i = 0; i < v.board_height; i++) {
		for (size_t j = 0; i < v.board_width; i++) {
			os << v.field[i][j] << " ";
		}
	}
	os << "\n";
	return os;
}

std::ostream &operator<<(std::ostream &os, const t_movement_result &v)
{
	os << "\nis_movable: " << v.is_slidable << "\nboard: " << v.board;
	return os;
}

std::ostream &operator<<(std::ostream &os, const t_game &v)
{
	os << "\nrandom_seed: " << v.random_seed << "\nboard_height: " << v.board_height
	   << "\nboard_width" << v.board_width << "\nhas_won: " << v.has_won
	   << "\nhas_lose: " << v.has_lose << "\nscore: " << v.score
	   << "\ncurrent_board: " << v.current_board << "\nmovement_results: \n"
	   << v.movement_results[0] << v.movement_results[1] << v.movement_results[2]
	   << v.movement_results[3];
	return os;
}