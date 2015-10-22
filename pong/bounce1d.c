/*
 *bounce1d.c :用户可以控制方向和速度
 *compile : gcc bounce1d.c set_ticker.c -lcurses -o bounce1d
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdlib.h>

#define MESSAGE "hello"
#define BLANK "     "

int row; //行数
int col; //列数
int dir; //方向
void move_msg(int);
extern int COLS;

int main()
{
     int delay;
     int ndelay;
     int c; //用户输入的字符

     initscr();
     cbreak();
     noecho();
     clear();

     row = 10;
     col = 0;
     dir = 1;
     delay = 200; //200ms = 0.2s

     move(row, col);
     addstr(MESSAGE);
     signal(SIGALRM, move_msg);
     set_ticker(delay);

     while(1)
     {
	  ndelay = 0;
	  c = getch();
	  if (c == 'q') break;
	  if (c == ' ') dir = -dir;
	  if (c == 'f' && delay > 2) ndelay = delay / 2;
	  if (c == 's') ndelay = delay * 2;
	  if (ndelay > 0)
	       set_ticker(delay = ndelay);
     }
     endwin();
}

void move_msg(int signum)
{
     signal(SIGALRM, move_msg); /*以防万一*/
     move(row, col);
     addstr(BLANK);
     col += dir;
     move(row, col);
     addstr(MESSAGE);
     refresh();

     if (dir == -1 && col <= 0)
	  dir = 1;
     else if (dir == 1 && col + strlen(MESSAGE) >= COLS )
	  dir = -1;
}
     
