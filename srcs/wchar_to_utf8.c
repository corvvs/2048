#include <stdint.h>
#include <wchar.h>

#include "ft_utf8.h"

uint32_t encode_2bytes_utf8(wchar_t wc)
{
	t_utf8_2 u = {wc};
	uint32_t first = 0b110 << 5 | u.second_lower << 2 | u.first_upper;
	uint32_t second = 0b10 << 6 | u.first_lower;
	return first | second << 8;
}

uint32_t encode_3bytes_utf8(wchar_t wc)
{
	t_utf8_3 u = {wc};
	uint32_t first =  0b1110 << 4 | u.second_upper;
	uint32_t second = 0b10 << 6 | u.second_lower << 2 | u.first_upper;
	uint32_t third = 0b10 << 6 | u.first_lower;
	return  first | second << 8 | third << 16;
}

uint32_t encode_4bytes_utf8(wchar_t wc)
{
	t_utf8_4 u = {wc};
	uint32_t first = 0b11110 << 3 | u.third_upper;
	uint32_t second = 0b10 << 6 | u.third_lower << 4 | u.second_upper;
	uint32_t third = 0b10 << 6 | u.second_lower << 2 | u.first_upper;
	uint32_t fourth = 0b10 << 6 | u.first_lower;
	return first | second << 8 | third << 16 | fourth << 24;
}
