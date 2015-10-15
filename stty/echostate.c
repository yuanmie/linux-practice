/*
 *报告终端回显位的状态.
 */
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>

int main()
{
     struct termios info;
     int rv;

     //获取终端的状态信息
     rv = tcgetattr(0, &info);
     if (rv == -1)
     {
	  perror("tcgetattr");
	  exit(1);
     }

     //测试终端回显位是否为1
     if ( info.c_lflag & ECHO)
	  printf("echo is on, since its bit is 1\n");
     else
	  printf("echo is off, since its bit is 0\n");
}
