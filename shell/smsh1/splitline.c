/*
 *splitline.c:读取下一个命令，将一个命令处理成一个字符串数组
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smsh.h"

/*
 * 从fp中读取下一条命令
 */
char *next_cmd(char *prompt, FILE *fp)
{
     char *buf; 
     int bufspace = 0; //总大小
     int pos = 0; //现在的位置
     int c;

     printf("%s", prompt);
     while ((c = getc(fp)) != EOF)
     {
	  //需要空间
	  if (pos + 1 > bufspace)
	  {
	       //第一次分配内存
	       if (bufspace == 0)
		    buf = emalloc(BUFSIZ);
	       //扩充
	       else
		    buf = erealloc(buf, bufspace + BUFSIZ);
	       bufspace += BUFSIZ; //更新大小
	  }

	  //命令的结束
	  if (c == '\n')
	       break;

	  //如果不是结束，则加入buf中
	  buf[pos ++] = c;
     }

     if (c == EOF && pos == 0) //EOF或者没有输入
	  return NULL;
     buf[pos] = '\0';
     return buf;
}

#define is_delim(x) ((x) == ' ' || (x) == '\t')

/*
 *解析为字符串数组
 */
char ** splitline(char *line)
{
     char **args;
     int spots = 0;
     int bufspace = 0; //总空间
     int argnum = 0; //参数个数
     char *cp = line; //指针
     char *start;
     int len;

     if (line == NULL)
	  return NULL;

     args = emalloc(BUFSIZ); //初始化数组
     bufspace = BUFSIZ;
     spots = BUFSIZ/sizeof(char *); //个数

     while (*cp != '\0')
     {
	  while(is_delim(*cp)) //跳过无关的字符
	       cp++;
	  if (*cp == '\0')
	       break;

	  //确保数组还有空间
	  if (argnum + 1 >= spots)
	  {
	       args = erealloc(args, bufspace + BUFSIZ);
	       bufspace += BUFSIZ;
	       spots += (BUFSIZ/sizeof(char *));
	  }

	  start = cp;
	  len = 1;
	  while (*++ cp != '\0' && !(is_delim(*cp)))
	       len ++;
	  args[argnum++] = newstr(start, len);
     }
     args[argnum] = NULL;
     return args;
}

char *newstr(char *s, int l)
{
     char *rv = emalloc(l + 1);
     rv[l] = '\0';
     strncpy(rv, s, l);
     return rv;
}

void freelist(char **list)
{
     char **cp = list;
     while (*cp)
	  free(*cp++);
     free(list);
}

void *emalloc(size_t n)
{
     void *rv;
     if ((rv = malloc(n)) == NULL)
	  fatal("out of memory", "", 1);
     return rv;
}

void *erealloc(void *p, size_t n)
{
     void *rv;
     if ((rv = realloc(p, n)) == NULL)
	  fatal("realloc() failed", "", 1);
     return rv;
}
