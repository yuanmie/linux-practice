#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

ino_t get_inode(char *);
void printpathto(ino_t);
void inum_to_name(ino_t, char *, int);

int main()
{
     printpathto(get_inode("."));
     putchar('\n');
     return 0;
}

/*
 *打印结点的路径
 */
void printpathto(ino_t this_inode)
{
     ino_t my_inode;
     char its_name[BUFSIZ];
     if (get_inode("..") != this_inode)
     {
	  chdir("..");
	  inum_to_name(this_inode, its_name, BUFSIZ); //获取结点文件的名字
	  my_inode = get_inode("."); //获取当前目录的结点
	  printpathto(my_inode); //递归调用printpathto
	  printf("/%s", its_name);
     }
}

/*
 *获取inode_to_find的名字
 */
void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen)
{
     DIR *dir_ptr;
     struct dirent *direntp;
     dir_ptr = opendir("."); //打开目录
     if (dir_ptr == NULL)
     {
	  perror(".");
	  exit(1);
     }

     //找到符合的结点并读取文件的名字
     while ((direntp = readdir(dir_ptr)) != NULL)
     {
	  if (direntp->d_ino == inode_to_find)
	  {
	       strncpy(namebuf, direntp->d_name, buflen);
	       namebuf[buflen-1] = '\0';
	       closedir(dir_ptr);
	       return;
	  }
     }
     fprintf(stderr, "error looking for inum %d\n", inode_to_find);
     exit(1);
}

/*
 *由文件名获取到i-node
 */
ino_t get_inode(char *filename)
{
     
     struct stat info;
     if (stat(filename, &info) == -1)
     {
	  fprintf(stderr, "Cannot stat ");
	  perror(filename);
	  exit(1);
     }
     return info.st_ino;
}
