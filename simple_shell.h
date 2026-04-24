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
char *get_path_env(void);
char *find_in_path(char *cmd);
void child_process(char *cmd_path, char **argv, char *prog_name, int cmd_num);
int execute_command(char **argv, char *prog_name, int cmd_num);
void print_env(void);

#endif /*SIMPLE_SHELL_H*/
