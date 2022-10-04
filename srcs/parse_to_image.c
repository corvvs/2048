#include <ncurses.h>
#include <wchar.h>

#include "characters.h"
#include "ft_utils.h"
#include "image.h"

#define INFOMATION_SIZE 5
int get_usable_win_width(WINDOW *w)
{
	int height, width;

	getmaxyx(w, height, width); // errorハンドリング?
	int usable_height = max_int(0, height - INFOMATION_SIZE - 1);
	int usable_width  = min_int(width / 2 - 1, usable_height) * 2;
	// printw("x : [%d] y : [%d]\n", width, height);
	return min_int(usable_width, MAX_DISPLAY_SIZE);
}

static void set_payload(t_block_image_row *dest, score_type src_num, int width)
{
	char numstr[64];
	int  digit             = ft_utoa_len(src_num, numstr, 0);
	int  left_padding_size = max_int(0, (width - digit) / 2);
	for (int i = 0; i < digit; i++) {
		(*dest)[left_padding_size + i] = numstr[i];
	}
}

static void parse_to_block_image_text(score_type num, t_block_image *img, const t_image_size *size)
{
	int line_padding_size = size->block_height / 2;

	set_payload(&img->field[line_padding_size], num, size->block_width);
}

void init_image_size(t_image_size *size, const t_board *board, WINDOW *w)
{
	int longer         = max_int(board->board_height, board->board_width);
	int width          = get_usable_win_width(w);
	int delim_count    = longer + 1;
	size->board_width  = board->board_width;
	size->board_height = board->board_height;
	size->block_width  = max_int(0, (width - delim_count) / longer);
	size->block_height = max_int(1, size->block_width / 2);
}

#define MINIMUM_DIGIT_HEIGHT_FOR_AA 5
#define MINIMUM_DIGIT_WIDTH_FOR_AA 3

static unsigned int get_digits_num(score_type n)
{
	if (n == 0) {
		return 1;
	}
	unsigned int d = 0;
	while (n) {
		d += 1;
		n /= 10;
	}
	return d;
}

static double block_scale_factor(score_type num, const t_image_size *size)
{
	const double HB      = size->block_height;
	const double WB      = size->block_width;
	const double HF      = MINIMUM_DIGIT_HEIGHT_FOR_AA;
	const double WF      = MINIMUM_DIGIT_WIDTH_FOR_AA;
	const int    d       = get_digits_num(num);
	const double s_sup_h = (HB - 2) / HF;
	const double s_sup_w = (WB - 1 - d) / (WF * d);
	// スケールファクター in double
	double sf = (s_sup_h > s_sup_w) ? s_sup_w : s_sup_h;
	return sf;
}

static void
parse_to_block_image_aa(score_type num, t_block_image *img, const t_image_size *size, double sf)
{
	// <<AA描画アルゴリズム>>
	//
	// [値の定義]
	// - ブロックの高さ(px):    HB
	// - ブロックの幅(px):      WB
	// - 数字フォントの高さ(px): HF = 5
	// - 数字フォントの幅(px):  WF = 3
	// - スケールファクター:    s (表示される数字が数字フォントの何倍の大きさなのか; 未知数!!)
	// - 文字数:              d (numから決まる)
	// - 数字1文字の高さ(px)    HD = HF * s
	// - 数字1文字の幅(px)      WD = WF * s
	// - 数字部分の高さ(px)     HN = HD
	// - 数字部分の幅(px)       WN = (WD + 1) * d - 1
	// - 上パディング(px)      PU = (HB - HN) / 2
	// - 下パディング(px)      PD = (HB - HN) - PU
	// - 左パディング(px)      PL = (WB - WN) / 2
	// - 右パディング(px)      PR = (WB - WN) - PL
	// - i番目(0-index)の文字の上位置(px) y[i] = PU
	// - i番目(0-index)の文字の左位置(px) x[i] = PL + i * WD
	//
	// [1. スケールファクターの決定]
	// 「数字部分がブロック内に１ピクセル以上の余白を持って収まる」かつ「スケールファクターが1以上」という制限を数式にすると
	// - HB >= HN + 2 = HD + 2 = HF * s + 2
	// - WB >= WN + 2 = (WD + 1) * d - 1 + 2 = (WF * s + 1) * d + 1
	// - s >= 1
	// 整理すると
	// - (HB - 2) / HF >= s
	// - (WB - 1 - d) / (WF * d) >= s
	// - s >= 1
	// 実際の s は, これらの不等式を全て満たす最大の整数となる.

	const double HB = size->block_height;
	const double WB = size->block_width;
	const double HF = MINIMUM_DIGIT_HEIGHT_FOR_AA;
	const double WF = MINIMUM_DIGIT_WIDTH_FOR_AA;
	const int    d  = get_digits_num(num);
	// スケールファクター in unsigned int
	// unsigned int sfi = sf;
	sf = (double)(unsigned int)(2 * sf) / 2;

	// [2. 文字の基準位置の決定]
	const unsigned int HD = HF * sf;
	const unsigned int WD = WF * sf;
	const unsigned int HN = HD;
	const unsigned int WN = WD * d;
	const unsigned int PU = (HB - HN) / 2;
	const unsigned int PL = (WB - WN) / 2;

	// [3. 文字ごとにデータを入れていく]
	for (int i = d - 1; 0 <= i; --i, num /= 10) {
		int                    k     = num % 10;
		const t_aa_char_type **griph = digit_griphs[k];
		// printw("i = %d, k = %d, griph = %p\n", i, k, griph);
		for (unsigned int pi = 0; pi < HD; ++pi) {
			for (unsigned int pj = 0; pj < WD; ++pj) {
				unsigned int di = pi / sf;
				unsigned int dj = pj / sf;
				// printw("(pi, pj) = (%d, %d), (di, dj) = (%d, %d), c = '%c'\n", pi, pj, di, dj,
				// griph[di][dj]);
				if (griph[di][dj] == CHAR_PIXEL_VACANT) {
					continue;
				}
				unsigned int yi    = PU + pi;
				unsigned int xi    = PL + i * (WD + 1) - 1 + pj;
				img->field[yi][xi] = AA_BLOCK_CHAR;
			}
		}
	}
}

void parse_board_to_image(const t_board *board, t_image *image, WINDOW *w)
{
	init_image_size(&image->size, board, w);
	printw("bw: [%d] bh: [%d]\n", image->size.block_width, image->size.block_height);

	for (size_t i = 0; i < board->board_height; i++) {
		for (size_t j = 0; j < board->board_width; j++) {
			score_type num = board->field[i][j].score;
			if (num == VACANT_BLOCK) {
				// 何も書かない
				continue;
			}
			t_block_image *blk            = &image->board[i][j];
			blk->color                    = max_int(0, (get_lsb(num) - 1) % COLOR_VARIATION);
			double     sf                 = block_scale_factor(num, &image->size);
			const bool display_nums_in_aa = sf >= 1;
			if (display_nums_in_aa) {
				parse_to_block_image_aa(num, blk, &image->size, sf);
			} else {
				parse_to_block_image_text(num, blk, &image->size);
			}
		}
	}
}

t_image create_result_image(const t_board *board, WINDOW *w)
{
	t_image image = {}; // こっちでやってる
	parse_board_to_image(board, &image, w);
	return image;
}

// bonusで使うかも
void set_wchar(t_block_image_row *row, wchar_t c, int size)
{
	for (int i = 0; i < size; i++) {
		(*row)[i] = c;
	}
}
