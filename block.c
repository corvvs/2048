#include "types.h"

// ブロック *b を空の状態にする
static void erase_block(t_block *b)
{
	*b = (t_block){
		.has_unified = false,
		.score       = VACANT_BLOCK,
	};
}

// ブロック *from を *to に移動させる
void move_block(t_block *from, t_block *to)
{
	*to = *from;
	erase_block(from);
}

// ブロック *erased を *remaining に合体させ, *erased は削除する
void unify_block(t_block *remaining, t_block *erased)
{
	remaining->score += erased->score;
	remaining->has_unified = true;
	erase_block(erased);
}
