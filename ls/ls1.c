/*
 *ls1.c :ls命令的第一个版本
 */

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void do_ls(char *);

int main(int argc, char *argv[])
{
     if (argc == 1)
     {
	  do_ls(".");
     }
     else
     {
	  while (-- argc)
	  {
	       printf("%s:\n", *(++argv));
	       do_ls(*argv);
	  }
     }
     return 0;
}

/*
 *ls1现在只支持列出目录
 */
void do_ls(char *dirname)
{
     DIR *dir_ptr;
     struct dirent *direntp;

     //打开目录
     if ((dir_ptr = opendir(dirname)) == NULL)
     {
	  fprintf(stderr, "ls1:cannot open %s\n", dirname);
     }
     else
     {
	  //读取目录中的项的名字
	  while ((direntp = readdir(dir_ptr)) != NULL)
	       printf("%s\n", direntp->d_name);
	  closedir(dir_ptr);
     }
}
     
