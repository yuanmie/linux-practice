#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "smsh.h"

#define DFL_PROMPT ">"
extern char **environ;

int main()
{
     char *cmdline, *prompt, **arglist;
     int result;

     prompt = DFL_PROMPT;
     setup();

     while ((cmdline = next_cmd(prompt, stdin)) != NULL)
     {
	  if ((arglist = splitline(cmdline)) != NULL)
	  {
	       result = process(arglist);
	       freelist(arglist);
	  }
	  free(cmdline);
     }

     return 0;
}

/*
 *初始化shell
 */
void setup()
{
     VLenviron2table(environ);
     signal(SIGINT, SIG_IGN);
     signal(SIGQUIT, SIG_IGN);
}


void fatal(char *s1, char *s2, int n)
{
     fprintf(stderr, "Error:%s, %s\n", s1, s2);
     exit(n);
}
