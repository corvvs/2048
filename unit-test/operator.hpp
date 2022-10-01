#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include <iostream>

#include "types.h"

bool operator==(const t_block &lhs, const t_block &rhs);
bool operator==(const t_board &lhs, const t_board &rhs);
bool operator==(const t_game &lhs, const t_game &rhs);
bool operator==(const t_movement_result &lhs, const t_movement_result &rhs);

bool operator!=(const t_block &lhs, const t_block &rhs);
bool operator!=(const t_board &lhs, const t_board &rhs);
bool operator!=(const t_movement_result &lhs, const t_movement_result &rhs);
bool operator!=(const t_game &lhs, const t_game &rhs);

std::ostream &operator<<(std::ostream &os, const t_block &v);
std::ostream &operator<<(std::ostream &os, const t_board &v);
std::ostream &operator<<(std::ostream &os, const t_movement_result &v);
std::ostream &operator<<(std::ostream &os, const t_game &v);

#endif
