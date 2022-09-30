#include <ncurses.h>
#include <stdbool.h>

// エラーハンドリング
void init_ncurses()
{
	initscr();
	timeout(-1);
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
}

int main()
{
	init_ncurses();
	while (true) {
		int c = getch();
		flushinp();
		printw("[%d]", c);
	}
	endwin();
}

// while (true) {
// 	t_moved moved;

// 	put_new_value(g_board);
// 	print_board(g_board);
// 	create_moved_board_for_every_direction(g_board, &moved);
// 	if (!moved.can_move) {
// 		break;
// 	}
// 	int direction = Getchar(moved.movable_set);
// 	if (direction == EOF || direction == ESC) {
// 		break;
// 	}
// 	g_board = moved.boards[direction];
// 	print_board(g_board);
// 	if (!is_victory(g_board)) {
// 		continue;
// 	}
// 	int res = display_dialogue();
// 	if (res == end) {
// 		break;
// 	}
// }
