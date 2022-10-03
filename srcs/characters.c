#include <stdlib.h>

// すべて 5 x 3
#ifndef BONUS
const char *griph_0[] = {
	"###\n",
	"# #\n",
	"# #\n",
	"# #\n",
	"###\n",
};

const char *griph_1[] = {
	"  #\n",
	"  #\n",
	"  #\n",
	"  #\n",
	"  #\n",
};

const char *griph_2[] = {
	"###\n",
	"  #\n",
	"###\n",
	"#  \n",
	"###\n",
};

const char *griph_3[] = {
	"###\n",
	"  #\n",
	"###\n",
	"  #\n",
	"###\n",
};

const char *griph_4[] = {
	"# #\n",
	"# #\n",
	"###\n",
	"  #\n",
	"  #\n",
};

const char *griph_5[] = {
	"###\n",
	"#  \n",
	"###\n",
	"  #\n",
	"###\n",
};

const char *griph_6[] = {
	"###\n",
	"#  \n",
	"###\n",
	"# #\n",
	"###\n",
};

const char *griph_7[] = {
	"###\n",
	"  #\n",
	"  #\n",
	"  #\n",
	"  #\n",
};

const char *griph_8[] = {
	"###\n",
	"# #\n",
	"###\n",
	"# #\n",
	"###\n",
};

const char *griph_9[] = {
	"###\n",
	"# #\n",
	"###\n",
	"  #\n",
	"###\n",
};

const char **digit_griphs[] = {
	griph_0,
	griph_1,
	griph_2,
	griph_3,
	griph_4,
	griph_5,
	griph_6,
	griph_7,
	griph_8,
	griph_9,
};
#else
const wchar_t *griph_0[] = {
	L"███\n",
	L"█ █\n",
	L"█ █\n",
	L"█ █\n",
	L"███\n",
};

const wchar_t *griph_1[] = {
	L"  █\n",
	L"  █\n",
	L"  █\n",
	L"  █\n",
	L"  █\n",
};

const wchar_t *griph_2[] = {
	L"███\n",
	L"  █\n",
	L"███\n",
	L"█  \n",
	L"███\n",
};

const wchar_t *griph_3[] = {
	L"███\n",
	L"  █\n",
	L"███\n",
	L"  █\n",
	L"███\n",
};

const wchar_t *griph_4[] = {
	L"█ █\n",
	L"█ █\n",
	L"███\n",
	L"  █\n",
	L"  █\n",
};

const wchar_t *griph_5[] = {
	L"███\n",
	L"█  \n",
	L"███\n",
	L"  █\n",
	L"███\n",
};

const wchar_t *griph_6[] = {
	L"███\n",
	L"█  \n",
	L"███\n",
	L"█ █\n",
	L"███\n",
};

const wchar_t *griph_7[] = {
	L"███\n",
	L"  █\n",
	L"  █\n",
	L"  █\n",
	L"  █\n",
};

const wchar_t *griph_8[] = {
	L"███\n",
	L"█ █\n",
	L"███\n",
	L"█ █\n",
	L"███\n",
};

const wchar_t *griph_9[] = {
	L"███\n",
	L"█ █\n",
	L"███\n",
	L"  █\n",
	L"███\n",
};

const wchar_t **digit_griphs[] = {
	griph_0,
	griph_1,
	griph_2,
	griph_3,
	griph_4,
	griph_5,
	griph_6,
	griph_7,
	griph_8,
	griph_9,
};
#endif
