#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#include "ft_utf8.h"

int ft_utoa_len(uint64_t n, char *s, int len)
{
	if (n > 9)
		len = ft_utoa_len(n / 10, s, len);
	s[len++] = "0123456789"[n % 10];
	return (len);
}

int min_int(int a, int b)
{
	return a < b ? a : b;
}

int max_int(int a, int b)
{
	return a > b ? a : b;
}

int get_lsb(uint64_t n)
{
	int i = 1;
	while (n) {
		if (n & 1) {
			return i;
		}
		n >>= 1;
		i++;
	}
	return 0;
}

void ft_putwchar_fd(int fd, wchar_t wc)
{
	if (wc <= 0x7F) {
		write(fd, &wc, 1);
	} else if (wc <= 0x7ff) {
		uint32_t c = encode_2bytes_utf8(wc);
		write(fd, &c, 2);
	} else if (wc <= 0xffff) {
		uint32_t c = encode_3bytes_utf8(wc);
		write(fd, &c, 3);
	} else if (wc <= 0x10ffff) {
		uint32_t c = encode_4bytes_utf8(wc);
		write(fd, &c, 4);
	}
}
