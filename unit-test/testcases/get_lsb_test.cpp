#include "gtest.h"
#include "util.h"

TEST(util, get_lsb)
{
	EXPECT_EQ(get_lsb(0), 0);
	for (int i = 0; i < 64; i++) {
		EXPECT_EQ(get_lsb(1ULL << i), i + 1);
	}
	for (int i = 0; i + 1 < 64; i++) {
		EXPECT_EQ(get_lsb((1ULL << i) | (1ULL << (i + 1))), i + 1);
	}
}