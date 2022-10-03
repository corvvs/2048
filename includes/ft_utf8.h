#ifndef FT_UTF8_H
#define FT_UTF8_H

#include <stdint.h>
#include <wchar.h>

typedef union {
	wchar_t w;
	struct {
		uint32_t first_lower   : 6;
		uint32_t first_upper   : 2;
		uint32_t second_lower  : 3;
		uint32_t second_unused : 5;
	};
} t_utf8_2;

typedef union {
	wchar_t w;
	struct {
		uint32_t first_lower  : 6;
		uint32_t first_upper  : 2;
		uint32_t second_lower : 4;
		uint32_t second_upper : 4;
	};
} t_utf8_3;

typedef union {
	wchar_t w;
	struct {
		uint32_t first_lower  : 6;
		uint32_t first_upper  : 2;
		uint32_t second_lower : 4;
		uint32_t second_upper : 4;
		uint32_t third_lower  : 2;
		uint32_t third_upper  : 3;
		uint32_t third_unused : 3;
	};
} t_utf8_4;

uint32_t encode_2bytes_utf8(wchar_t wc);
uint32_t encode_3bytes_utf8(wchar_t wc);
uint32_t encode_4bytes_utf8(wchar_t wc);
#endif
