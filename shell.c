#include "simple_shell.h"

/**
 * display_prompt - Prints the shell prompt to stdout
 *
 * Only displays when stdin is connected to a terminal.
 *
 * Return: void
 */
void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, PROMPT, 2);
}

/**
 * read_line - Reads one line of input from stdin using getline
 *
 * Allocates a buffer via getline. The caller is responsible for
 * freeing the returned pointer. Prints a newline on EOF when stdin
 * is a terminal.
 *
 * Return: pointer to the allocated line, or NULL on EOF / error
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
 * strip_newline - Removes the trailing newline character from a string
 * @str: the string to modify in place
 *
 * Searches for the first newline and replaces it with a null byte.
 *
 * Return: void
 */
void strip_newline(char *str)
{
	char *newline;

	newline = strchr(str, '\n');
	if (newline)
		*newline = '\0';
}

/**
 * execute_command - Forks and executes a command with its arguments
 * @argv: NULL-terminated array where argv[0] is the executable path
 *        and the remaining elements are its arguments
 *
 * Prints an error message to stderr if the executable cannot be
 * found or executed. The parent waits for the child to complete.
 *
 * Return: void
 */
void execute_command(char **argv)
{
	pid_t pid;
	int status;

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
			fprintf(stderr, "./simple_shell: No such file or directory: %s\n",
				argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
	}
}

/**
 * main - Entry point for simple_shell
 *
 * Repeatedly displays a prompt, reads a command line, splits it into
 * tokens, and executes it until EOF is received or a read error occurs.
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

		strip_newline(line);

		if (line[0] != '\0')
			execute_command(line);

		free(line);
	}

	return (0);
}
