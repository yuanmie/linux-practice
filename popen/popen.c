/*
 *popen.c:
 */

#include <stdio.h>
#include <signal.h>

#define READ 0
#define WRITE 1

FILE *popen(const char *command, const char *mode)
{
     int pfp[2], pid;
     FILE *fp;
     int parent_end, child_end;

     if (*mode == 'r')
     {
	  parent_end = READ;
	  child_end = WRITE;
     }else if (*mode == 'w')
     {
	  parent_end = WRITE;
	  child_end = READ;
     }else return NULL;

     //建立管道
     if (pipe(pfp) == -1)
	  return NULL;

     //fork子进程
     if ((pid = fork()) == -1)
     {
	  close(pfp[0]);
	  close(pfp[1]);
	  return NULL;
     }

     //父进程代码
     if (pid > 0)
     {
	  if (close(pfp[child_end]) == -1)
	       return NULL;
	  return fdopen(pfp[parent_end], mode);
     }

     //子进程代码
     if (close(pfp[parent_end] == -1))
	 exit(1);

     if (dup2(pfp[child_end], child_end) == -1)
	  exit(1);

     if (close(pfp[child_end]) == -1)
	  exit(1);

     execl("/bin/sh", "sh", "-c", command, NULL);
     exit(1);
}
	 
int main()
{
     //test
     char buf[100];
     FILE *fp = popen("ls", "r");
     int i = 0;
     while(fgets(buf, 100, fp) != NULL)
	  printf("%3d %s", i++, buf);
     
     pclose(fp);
     return 0;
}
	  
