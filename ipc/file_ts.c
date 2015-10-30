/*
 *基于文件的时间服务器
 *学习文件锁的使用
 */

#include <stdio.h>
#include <sys/file.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

#define oops(m,x) {perror(m);exit(x);}
void lock_operation(int fd, int op);

int main(int argc, char *argv[])
{
     int fd;
     time_t now;
     char *message;

     if (argc != 2)
     {
	  fprintf(stderr, "usage:file_ts filename\n");
	  exit(1);
     }

     if ((fd = open(argv[1], O_CREAT|O_TRUNC|O_WRONLY, 0644)) == -1)
	  oops(argv[1], 2);

     while (1)
     {
	  time(&now);
	  message = ctime(&now);

	  lock_operation(fd, F_WRLCK);
	  if (lseek(fd, 0L, SEEK_SET) == -1)
	       oops("lseek", 3);
	  if (write(fd, message, strlen(message)) == -1)
	       oops("write", 4);

	  lock_operation(fd, F_UNLCK);
	  sleep(1);
     }
}

void lock_operation(int fd, int op)
{
     struct flock lock;
     lock.l_whence = SEEK_SET;
     lock.l_start = lock.l_len = 0;
     lock.l_pid = getpid();
     lock.l_type = op;

     if (fcntl(fd, F_SETLKW, &lock) == -1)
	  oops("lock operation", 6);
}
