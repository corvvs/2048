#include <limits.h>
#include <stddef.h>
#include <stdint.h>

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
