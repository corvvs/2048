#include "ft_utils.h"
#include "types.h"
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define FILENAME_HIGHSCORE "./.highscore.2048"
#define SCORE_MAX UINT64_MAX

#ifdef BONUS
static bool my_atou64(const char *str, score_type *n)
{
	*n = 0;
	for (size_t i = 0; '0' <= str[i] && str[i] <= '9'; ++i) {
		score_type d = str[i] - '0';
		// *n * 10 + d > SCORE_MAX
		if (*n > SCORE_MAX / 10 || (d > SCORE_MAX - *n * 10)) {
			return false;
		}
		*n = *n * 10 + d;
	}
	return true;
}
#endif

score_type get_highscore()
{
#ifndef BONUS
	return SCORE_UNAVAILABLE;
#else
	int fd = open(FILENAME_HIGHSCORE, O_RDONLY);
	if (fd < 0) {
		return SCORE_UNAVAILABLE;
	}
	char    buf[1025];
	ssize_t read_size = read(fd, buf, ARRAY_LEN(buf, char) - 1);
	close(fd);
	if (read_size <= 0) {
		return SCORE_UNAVAILABLE;
	}
	// - FILENAME_HIGHSCORE からデータが読み込めること
	// - ↑ の内容が /^(\d+)\n?$/ にマッチすること
	// - ↑ が score_type にオーバーフローしないで変換できること
	// 以上ができる場合, 変換結果を返す.

	ssize_t i = 0;
	for (; i < read_size; ++i) {
		char c = buf[i];
		if (c < '0' || '9' < c) {
			break;
		}
	}
	if (i == 0) {
		return SCORE_UNAVAILABLE;
	}
	if (!((i == read_size) || (i + 1 == read_size && buf[i] == '\n'))) {
		return SCORE_UNAVAILABLE;
	}
	buf[i] = '\0';
	score_type n;
	const bool succeeded = my_atou64(buf, &n);
	if (!succeeded) {
		return SCORE_UNAVAILABLE;
	}
	return n;
#endif
}

void set_highscore(score_type score)
{
#ifndef BONUS
	(void)score;
	return;
#else
	score_type high_score = get_highscore();
	if (high_score != SCORE_UNAVAILABLE && high_score >= score) {
		return;
	}
	int fd = open(FILENAME_HIGHSCORE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0) {
		return;
	}
	char buf[1024];
	int  len = ft_utoa_len(score, buf, 0);
	buf[len] = '\n';
	write(fd, buf, len + 1);
	close(fd);
#endif
}
