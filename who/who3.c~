/*
 *who2.c 实现linux中who命令的功能
 *
 */

#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SHOWHOST
void show_info(struct utmp*);
void showtime(long);

int main()
{
     struct utmp current_record; //向这里读入信息
     int utmpfd; //读取的文件描述符
     int reclen = sizeof(current_record);

     //如果文件打开失败，则打印出错误信息并结束
     if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)
     {
	  perror(UTMP_FILE);  //UTMP_FILE is in utmp.h
	  exit(1);
     }

     /*
      *从文件的读取数据填充到current_record中
      *并打印出来
      */
     while (read(utmpfd, &current_record, reclen) == reclen)
	  show_info(&current_record);

     close(utmpfd);
     return 0;
}

/*
 *show_info()
 *以人类可读的形式输出utmp_record中的内容
 */

void show_info(struct utmp *utbufp)
{
     //不是已登录用户，则返回
     if (utbufp->ut_type != USER_PROCESS)
	  return;
     
     printf("%-8.8s", utbufp->ut_name); /* 登录名 */
     printf(" ");
     printf("%-8.8s", utbufp->ut_line); /* 终端名 */
     printf(" ");
     showtime(utbufp->ut_time); //时间
     printf(" ");
#ifdef SHOWHOST
     printf("(%s)", utbufp->ut_host); /*主机名*/
#endif
     printf("\n");
}

/*showtime(long)
 *以一种方便阅读的格式输出时间
 */

void showtime(long timeval)
{
     char *cp;
     cp = ctime(&timeval);
     printf("%12.12s", cp+4);
}
