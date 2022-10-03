#ifndef CHARACTERS_H
#define CHARACTERS_H

#define FONT_HEIGHT 5
#define FONT_WIDTH 5

#ifndef BONUS
  #define CHAR_PIXEL_VACANT ' '
  #define AA_BLOCK_CHAR '#'
typedef char t_aa_char_type;
#else
  #define CHAR_PIXEL_VACANT L' '
  #define AA_BLOCK_CHAR L'█'
typedef wchar_t t_aa_char_type;
#endif

extern const t_aa_char_type **digit_griphs[];

#endif
