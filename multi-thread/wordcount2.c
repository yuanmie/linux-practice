/*
 *利用互斥量
 */

#include <stdio.h>
#include <pthread.h>
#include <ctype.h>

int total_words;
pthread_mutex_t counter_lock = PTHREAD_MUTEX_INITIALIZER;
void count_words(char *f);

main(int argc, char *argv[])
{
     pthread_t t1, t2;
     if (argc != 3)
     {
	  printf("usage:%s file file2\n", argv[0]);
	  exit(1);
     }
     total_words = 0;
     pthread_create(&t1, NULL, count_words, argv[1]);
     pthread_create(&t2, NULL, count_words, argv[2]);
     pthread_join(t1, NULL);
     pthread_join(t2, NULL);
     printf("%5d:total words\n", total_words);

}

void count_words(char *f)
{
     char *filename = f;
     FILE *fp;
     int c, prevc = '\0';

     if ((fp = fopen(filename, "r")) != NULL)
     {
	  while ((c = getc(fp)) != EOF)
	  {
	       if (!isalnum(c) && isalnum(prevc))
	       {
		    pthread_mutex_lock(&counter_lock);
		    total_words++;
		    pthread_mutex_unlock(&counter_lock);
	       }
	       prevc = c;
	  }
	  fclose(fp);
     }else
     {
	  perror(filename);
     }
     return;
}
