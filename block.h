#ifndef BLOCK_H
#define BLOCK_H

#include "types.h"

void move_block(t_block *from, t_block *to);
void unify_block(t_block *remaining, t_block *erased);

#endif
