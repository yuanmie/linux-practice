/*
 *关闭了终端的回显
 *让终端没有延迟
 */

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#define ASK "Do you want another transaction"
#define TRIES 3 //最大尝试次数
#define SLEEPTIME 2 //每次尝试的时间
#define BEEP putchar('\a'); //提醒用户
void ctrl_c_handler(int);
void tty_mode(int);
int get_response(char *, int);
int get_ok_char();
void set_cr_noecho_mode();
void set_nodelay_mode();

int main()
{
     int response;
     void ctrl_c_handler(int);
     void tty_mode(int);

     tty_mode(0);
     set_cr_noecho_mode();
     set_nodelay_mode();
     signal(SIGINT, ctrl_c_handler);
     signal(SIGQUIT, SIG_IGN);
     response = get_response(ASK, TRIES);
     tty_mode(1);
     return response;
}

/*
 *获取回答
 */
int get_response(char *question, int maxtries)
{
     int input;

     printf("%s(y/n)?\n", question);
     fflush(stdout);
     while(1)
     {
	  sleep(SLEEPTIME);
	  input = tolower(get_ok_char(0));
	  if (input == 'y')
	       return 0;
	  if (input == 'n')
	       return 1;
	  if (maxtries -- ==0)
	       return 2;
	  BEEP;
     }
}

int get_ok_char()
{
     int c;
     while ((c = getchar()) != EOF && strchr("yYnN", c) == NULL)
	  ;
     return c;
}

/*
 *一个字符字符的输入，并且关闭回显
 */
void set_cr_noecho_mode()
{
     struct termios ttystate;

     tcgetattr(0, &ttystate); //读取设置
     ttystate.c_lflag &= ~ICANON; //没有缓冲
     ttystate.c_lflag &= ~ECHO; //没有回显
     ttystate.c_cc[VMIN] = 1; //一次只获取一个字符
     tcsetattr(0, TCSANOW, &ttystate); //安装设置
}

void set_nodelay_mode()
{
     int termflags; 
     termflags = fcntl(0, F_GETFL); //读取位置
     termflags |= O_NDELAY; //翻转nodelay位
     fcntl(0, F_SETFL, termflags); //安装设置
}

/*
 *当how为0时，保存当前的设置，当how为1时，还原当时保存的设置
 */
void tty_mode(int how)
{
     static struct termios original_mode;
     static int original_flags;
     static int stored = 0;

     if (how == 0)
     {
	  tcgetattr(0, &original_mode);
	  original_flags = fcntl(0, F_GETFL);
	  stored = 1;
     }
     else if (stored)
     {
	  tcsetattr(0, TCSANOW, &original_mode);
	  fcntl(0, F_SETFL, original_flags);
     }
}

/*
 *当中断产生时，调用这个函数再退出程序
 */
void ctrl_c_handler(int signum)
{
     tty_mode(1);
     exit(1);
}
			  
