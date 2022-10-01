#include "gtest.h"

#include "game.h"
#include "operator.hpp"
#include "types.h"

TEST(init_game, init)
{
	t_game g1;
	t_game g2;
	init_game(&g1, 1, 3, 4);
	init_game(&g2, 1, 3, 4);
	EXPECT_EQ(g1, g2);
	init_game(&g1, 1, 3, 4);
	init_game(&g2, 1, 3, 3);
	EXPECT_NE(g1, g2);
}