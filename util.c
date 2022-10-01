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
