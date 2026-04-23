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
 * Return: array of tokens
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
 * execute_command - forks and executes a command
 * @argv: argument vector
 *
 * Return: void
 */
void execute_command(char **argv)
{
	pid_t pid;
	int status;

	if (argv[0] == NULL)
		return;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0)
	{
		if (execve(argv[0], argv, environ) == -1)
		{
			fprintf(stderr, "./simple_shell: 1: %s: not found\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
	}
}

/**
 * main - entry point for simple shell
 *
 * Return: Always 0
 */
int main(void)
{
	char *line;
	char **argv;

	while (1)
	{
		display_prompt();

		line = read_line();
		if (line == NULL)
			break;

		argv = split_line(line);
		execute_command(argv);

		free(argv);
		free(line);
	}

	return (0);
}
