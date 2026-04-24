#include "simple_shell.h"

/**
 * display_prompt - prints the shell prompt to stdout
 *
 * Return: void
 */
void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, PROMPT, 2);
}

/**
 * read_line - reads one line from stdin
 *
 * Return: pointer to input line, or NULL on EOF
 */
char *read_line(void)
{
	char *line;
	size_t len;
	ssize_t nread;

	line = NULL;
	len = 0;
	nread = getline(&line, &len, stdin);

	if (nread == -1)
	{
		free(line);
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "\n", 1);
		return (NULL);
	}

	return (line);
}

/**
 * split_line - splits a line into tokens
 * @line: line to split
 *
 * Return: array of tokens, or NULL on failure
 */
char **split_line(char *line)
{
	int bufsize, position;
	char **tokens;
	char *token;

	bufsize = TOK_BUFSIZE;
	position = 0;
	tokens = malloc(sizeof(char *) * bufsize);
	if (tokens == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, TOK_DELIM);
	while (token != NULL)
	{
		tokens[position] = token;
		position++;

		if (position >= bufsize)
		{
			bufsize += TOK_BUFSIZE;
			tokens = realloc(tokens, sizeof(char *) * bufsize);
			if (tokens == NULL)
			{
				perror("realloc");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, TOK_DELIM);
	}

	tokens[position] = NULL;
	return (tokens);
}

/**
 * main - entry point for simple shell
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: Always 0
 */
int main(int argc, char **argv)
{
	char *line;
	char **args;
	int last_status, cmd_num;

	(void)argc;
	last_status = 0;
	cmd_num = 1;

	while (1)
	{
		display_prompt();

		line = read_line();
		if (line == NULL)
			break;

		args = split_line(line);

		if (args[0] != NULL && strcmp(args[0], "exit") == 0)
		{
			free(args);
			free(line);
			return (last_status);
		}

		last_status = execute_command(args, argv[0], cmd_num);
		cmd_num++;

		free(args);
		free(line);
	}

	return (last_status);
}
