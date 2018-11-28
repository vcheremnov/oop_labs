#include <iostream>
#include "game.h"

#include <ncurses.h>

//#define QUEEN_CHAR ACS_CKBOARD

//int *nqueens(int num);
//int place(int current, int *position);
//int print();
//void board(WINDOW *win, int starty, int startx, int lines, int cols,
//       int tile_width, int tile_height);

//int main()
//{
//    initscr();
//    curs_set(0);
//    cbreak();
//    keypad(stdscr, TRUE);
//    print();
//    endwin();
//    return 0;
//}

//int print()
//{
//    int y = 0, x = 0, w = 4, h = 2;
//    static int solution = 1;

//    board(stdscr, y, x, 10, 10, w, h);
//    refresh();
//    if(getch() == KEY_F(1))
//    {	endwin();
//        exit(0);
//    }
//    clear();
//}

void board(WINDOW *win, int starty, int startx, int lines, int cols,
       int tile_width, int tile_height)
{	int endy, endx, i, j;

    endy = starty + lines * tile_height;
    endx = startx + cols  * tile_width;

    for(j = starty; j <= endy; j += tile_height)
        for(i = startx; i <= endx; ++i)
            mvwaddch(win, j, i, ACS_HLINE);
    for(i = startx; i <= endx; i += tile_width)
        for(j = starty; j <= endy; ++j)
            mvwaddch(win, j, i, ACS_VLINE);
    mvwaddch(win, starty, startx, ACS_ULCORNER);
    mvwaddch(win, endy, startx, ACS_LLCORNER);
    mvwaddch(win, starty, endx, ACS_URCORNER);
    mvwaddch(win, 	endy, endx, ACS_LRCORNER);
    for(j = starty + tile_height; j <= endy - tile_height; j += tile_height)
    {	mvwaddch(win, j, startx, ACS_LTEE);
        mvwaddch(win, j, endx, ACS_RTEE);
        for(i = startx + tile_width; i <= endx - tile_width; i += tile_width)
            mvwaddch(win, j, i, ACS_PLUS);
    }
    for(i = startx + tile_width; i <= endx - tile_width; i += tile_width)
    {	mvwaddch(win, starty, i, ACS_TTEE);
        mvwaddch(win, endy, i, ACS_BTEE);
    }
    wrefresh(win);
}

int main()
{
    Game seaBattle(GameType::ConsoleGame);
    seaBattle.run();

    return 0;
}
