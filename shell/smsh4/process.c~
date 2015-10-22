/*
 *process.c: 解析语句中的内置函数（如exit（），set），还有控制结构（if，while）
 */
#include <stdio.h>
#include "smsh.h"

int is_control_command(char *);
int do_control_command(char **);
int ok_to_excetute();

int process(char **args)
{
     int rv = 0;
     if (args[0] == NULL)
	  rv = 0;
     else if (is_control_command(args[0]))
	  rv = do_control_command(args);
     else if (ok_to_execute())
	  rv = execute(args);
     return rv;
}

