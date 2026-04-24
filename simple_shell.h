#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT "$ "
#define TOK_DELIM " \t\r\n"
#define TOK_BUFSIZE 64

extern char **environ;

/*Prototypes*/
void display_prompt(void);
char *read_line(void);
char **split_line(char *line);
char *find_in_path(char *cmd);
void execute_command(char **argv);

#endif /*SIMPLE_SHELL_H*/
