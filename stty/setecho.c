/*
 *setecho.c:设置回显位的状态
 *usage: setecho[y][n]
 */

#include <stdio.h>
#include <termios.h>
#include <stdlib.h>

#define oops(s, x) {perror(s); exit(x);}

int main(int argc, char *argv[])
{
     struct termios info;
     if (argc == 1)
	  exit(0);

     //获取终端信息
     if (tcgetattr(0, &info) == -1)
	  oops("tcgetattr",1);

     //如果输入的参数为y，则打开回显
     if (argv[1][0] == 'y')
	  info.c_lflag |= ECHO; //设置回显位为1
     else
	  info.c_lflag &= ~ECHO; //设置回显位为0

     //设置立刻生效
     if (tcsetattr(0, TCSANOW, &info) == -1)
	  oops("tcsetattr", 2);
}

