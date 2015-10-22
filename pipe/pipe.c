/*
 *pipe.c :演示如何创建一个管道从一个进程到另一个进程
 *usage: pipe cmd1 cmd2
 *cmd1的输出当做cmd2的输入
 *相当与shell中的cmd1|cmd2
 *限制，命令不能带参数
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define oops(m, x) { perror(m); exit(x);}
int main(int argc, char *argv[])
{
     int thepipe[2]; //两个文件描述符
     int newfd;
     int pid;

     if (argc != 3)
     {
	  fprintf(stderr, "usage:pipe cmd1 cmd2\n");
	  exit(1);
     }
     if (pipe(thepipe) == -1) //得到管道
	  oops("Cannot get a pipe", 1);

     if ((pid = fork()) == -1) //创建子进程
	  oops("Cannot fork", 2);

     if (pid > 0) //父进程会执行argv[2]
     {
	  close(thepipe[1]); //父进程不向管道写东西
	  //重定向，将stdin与thepipe[0]连接起来
	  if (dup2(thepipe[0], 0) == -1)
	       oops("Could not redirect stdin", 3);

	  close(thepipe[0]);
	  execlp(argv[2], argv[2], NULL);
	  
	  oops(argv[2], 4);
     }

     printf("how many time it can run!\n");
     close(thepipe[0]);
     //重定向，将stdout与thepipe[1]连接起来
     if (dup2 (thepipe[1], 1) == -1)
	  oops("could not redirect stdout", 4);

     close(thepipe[1]);
     execlp(argv[1], argv[1], NULL);
     oops(argv[1], 5);
}
