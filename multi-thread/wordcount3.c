/*
 *利用互斥量
 */

#include <stdio.h>
#include <pthread.h>
#include <ctype.h>

int total_words;
struct arg_set{
     char * fname;
     int count;
};

void count_words(struct arg_set* );
main(int argc, char *argv[])
{
     pthread_t t1, t2;
     struct arg_set args1, args2;
     if (argc != 3)
     {
	  printf("usage:%s file file2\n", argv[0]);
	  exit(1);
     }
     total_words = 0;
     args1.fname = argv[1];
     args1.count = 0;
     pthread_create(&t1, NULL, count_words, &args1);
     args2.fname = argv[2];
     args2.count = 0;
     pthread_create(&t2, NULL, count_words, &args2);
     pthread_join(t1, NULL);
     pthread_join(t2, NULL);
     total_words = args1.count + args1.count;
     printf("%5d:total words\n", total_words);

}

void count_words(struct arg_set *a)
{
     FILE *fp;
     int c, prevc = '\0';

     if ((fp = fopen(a->fname, "r")) != NULL)
     {
	  while ((c = getc(fp)) != EOF)
	  {
	       if (!isalnum(c) && isalnum(prevc))
	       {
		    a->count++;
	       }
	       prevc = c;
	  }
	  fclose(fp);
     }else
     {
	  perror(a->fname);
     }
     return;
}
