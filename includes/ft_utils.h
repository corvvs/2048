#ifndef FT_UTILS_H
#define FT_UTILS_H

#include <stdint.h>
#include <wchar.h>

int ft_utoa_len(uint64_t n, char *s, int len);
int min_int(int a, int b);
int max_int(int a, int b);
int get_lsb(uint64_t n);
void ft_putwchar_fd(int fd, wchar_t wc);

#endif
