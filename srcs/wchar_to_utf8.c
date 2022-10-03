#include <stdint.h>
#include <wchar.h>

#include "ft_utf8.h"

typedef union {
	uint32_t dword;
	uint8_t  byte[4];
} t_dword;

uint32_t encode_2bytes_utf8(wchar_t wc)
{
	t_utf8_2 u       = {wc};
	t_dword  encoded = {0};
	encoded.byte[0]  = 0b110 << 5 | u.second_lower << 2 | u.first_upper;
	encoded.byte[1]  = 0b10 << 6 | u.first_lower;
	return encoded.dword;
}

uint32_t encode_3bytes_utf8(wchar_t wc)
{
	t_utf8_3 u       = {wc};
	t_dword  encoded = {0};
	encoded.byte[0]  = 0b1110 << 4 | u.second_upper;
	encoded.byte[1]  = 0b10 << 6 | u.second_lower << 2 | u.first_upper;
	encoded.byte[2]  = 0b10 << 6 | u.first_lower;
	return encoded.dword;
}

uint32_t encode_4bytes_utf8(wchar_t wc)
{
	t_utf8_4 u       = {wc};
	t_dword  encoded = {0};
	encoded.byte[0]  = 0b11110 << 3 | u.third_upper;
	encoded.byte[1]  = 0b10 << 6 | u.third_lower << 4 | u.second_upper;
	encoded.byte[2]  = 0b10 << 6 | u.second_lower << 2 | u.first_upper;
	encoded.byte[3]  = 0b10 << 6 | u.first_lower;
	return encoded.dword;
}
