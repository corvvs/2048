#include "types.h"

// ブロック *b を空の状態にする
static void erase_block(t_block *b)
{
	*b = (t_block){
		.has_unified = false,
		.score       = VACANT_BLOCK,
	};
}

void swap_block(t_block *a, t_block *b)
{
	t_block c = *a;
	*a        = *b;
	*b        = c;
}

// ブロック *erased を *remaining に合体させ, *erased は削除する
void unify_block(t_block *remaining, t_block *erased)
{
	remaining->score += erased->score;
	remaining->has_unified = true;
	erase_block(erased);
}
