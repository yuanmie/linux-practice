/*
 *utmplib.c - 从utmp 文件中读取数据的带缓冲的函数
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmp.h>

#define NRECS 16
#define NULLUT ((struct utmp *) NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];  //存储16条utmp结构大小的记录的缓冲区
static int num_recs; //缓冲区中记录的条数
static int cur_rec; //将要读取的记录的下标
static int fd_utmp = -1;//要读取的文件的文件描述符

int utmp_open(char *filename)
{
     //读取文件并初始化
     fd_utmp = open(filename, O_RDONLY);
     cur_rec = num_recs = 0;
     return fd_utmp;
}

struct utmp *utmp_next()
{
     struct utmp *recp;
     //如果文件读取失败，则返回null
     if (fd_utmp == -1)
	  return NULLUT;

     //没有下一条记录了，文件读取结束
     if (cur_rec == num_recs && utmp_reload() == 0)
	  return NULLUT;

     //读取记录，并且cur_rec加1
     recp = (struct utmp *)&utmpbuf[cur_rec * UTSIZE];
     cur_rec++;
     return recp;
}
		     
int utmp_reload()
{
     //读取记录并计算记录的条数，并让cur_rec初始为0
     int amt_read;
     amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE);
     num_recs = amt_read/UTSIZE;
     cur_rec = 0;
     return num_recs;
}

/*
 *关闭文件，如果它被打开了
 */
void utmp_close()
{
     if (fd_utmp != -1)
	  close(fd_utmp);
}
