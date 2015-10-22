/*
 *第2个版本shell
 *用fork()创建一个子进程，来执行命令，父进程使用wait()等待子进程结束
 */

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#define MAXARGS 20 //命令行参数，假设最多只有20个
#define ARGLEN 100 //字符串长度

char *makestring();

int main()
{
     char *arglist[MAXARGS + 1]; //字符串的数组
     int numargs; //arglist的下标
     char argbuf[ARGLEN];
     numargs = 0;
     while (numargs < MAXARGS)
     {
	  printf("Arg[%d]?", numargs);
	  if (fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
	       arglist[numargs ++] = makestring(argbuf);
	  //如果没有输入内容，直接回车
	  else
	  {
	       if (numargs > 0)
	       {
		    arglist[numargs] = NULL;
		    execute(arglist);
		    numargs = 0;
	       }
	  }
     }
     return 0;
}

int execute(char *arglist[])
{
     int pid, exitstatus;

     pid = fork();
     switch(pid)
     {
     case -1:
	  perror("fork failed");
	  exit(1);
     case 0:
	  execvp(arglist[0], arglist);
	  perror("execvp failed");
	  exit(1);
     default:
	  while (wait(&exitstatus) != pid)
	       ;
	  printf("child exited with status %d, %d\n",
		 exitstatus>>8, exitstatus&0377);
     }
}

char *makestring(char *buf)
{
     char *cp;
     buf[strlen(buf)-1] = '\0'; //消除'\n'
     cp = malloc(strlen(buf) + 1);
     if (cp == NULL)
     {
	  fprintf(stderr, "no memory\n");
	  exit(1);
     }
     strcpy(cp, buf);
     return cp;
}
     
     

