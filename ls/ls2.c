/*
 *ls2.c :ls命令的第二个版本
 */

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

void do_ls(char *);
void dostat(char *,char *);
void show_file_info(char *, struct stat *);
void mode_to_letters(int, char *);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);

int main(int argc, char *argv[])
{
     //如果没有参数，则默认列出当前目录
     if (argc == 1)
     {
	  do_ls(".");
     }
     else
     {
	  while (--argc)
	  {
	       printf("%s:\n", *(++argv));
	       do_ls(*argv);
	  }
     }
     return 0;
}

/*
 *列出目录中的文件
 */

void do_ls(char *dirname)
{
     DIR *dir_ptr; //目录指针
     struct dirent *direntp; //目录中的条目
     char *fullpath;

     //文件的全称
     fullpath = (char *)malloc(strlen(dirname) + 1 + MAXNAMLEN + 1);
     if ((dir_ptr = opendir(dirname)) == NULL)
     {
	  fprintf(stderr, "ls2:cannot open %s\n", dirname);
     }
     else
     {
	  while ((direntp = readdir(dir_ptr)) != NULL)
	  {
	       sprintf(fullpath, "%s/%s",dirname, direntp->d_name);
	       dostat(fullpath,direntp->d_name);
	  }
	  closedir(dir_ptr);
     }
}

void dostat(char *fullpath, char *filename)
{
     struct stat info;
     //获取文件状态
     if (stat(fullpath, &info) == -1)
     {
	  perror(filename);
     }
     else
     {
	  show_file_info(filename, &info);
     }
}

/*
 *显示文件的详细信息
 */

void show_file_info(char *filename, struct stat *info_p)
{
     char modestr[11];

     mode_to_letters(info_p->st_mode, modestr);

     printf("%s\t", modestr);
     printf("%4d\t", (int)info_p->st_nlink);
     printf("%-8s\t", uid_to_name(info_p->st_uid));
     printf("%-8s\t", gid_to_name(info_p->st_gid));
     printf("%8ld\t", (long)info_p->st_size);
     char * tim = ctime(&(info_p->st_mtim));
     printf("%.12s\t", 4 + ctime(&(info_p->st_mtim)));
     printf("%s\n", filename);
}

/*
 *将文件的权限位转换为字符表示
 */
void mode_to_letters(int mode, char str[])
{
     strcpy(str, "----------");
     if (S_ISDIR(mode)) str[0] = 'd';
     if (S_ISCHR(mode)) str[0] = 'c';
     if (S_ISBLK(mode)) str[0] = 'b';

     if (mode & S_IRUSR) str[1] = 'r';
     if (mode & S_IWUSR) str[2] = 'w';
     if (mode & S_IXUSR) str[3] = 'x';

     if (mode & S_IRGRP) str[4] = 'r';
     if (mode & S_IWGRP) str[5] = 'w';
     if (mode & S_IXGRP) str[6] = 'x';

     if (mode & S_IROTH) str[7] = 'r';
     if (mode & S_IWOTH) str[8] = 'w';
     if (mode & S_IXOTH) str[9] = 'x';
}

#include <pwd.h>
/*
 *根据uid获取用户名
 */

char *uid_to_name(uid_t uid)
{
     struct passwd *pw_ptr;
     static char numstr[10];

     if ((pw_ptr = getpwuid(uid)) == NULL)
     {
	  sprintf(numstr, "%d", uid);
	  return numstr;
     }
     else
     {
	  return pw_ptr->pw_name;
     }
}
     
#include <grp.h>
/*
 *根据gid获取组名
 */
    
char *gid_to_name(gid_t gid)
{
     struct group *grp_ptr;
     static char numstr[10];

     if ((grp_ptr = getgrgid(gid)) == NULL)
     {
	  sprintf(numstr, "%d", gid);
	  return numstr;
     }
     else
     {
	  return grp_ptr->gr_name;
     }
}
     
