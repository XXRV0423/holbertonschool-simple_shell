#include "simple_shell.h"

/**
 * main - Entry point for simple_shell
 *
 * Repeatedly display a prompt, reads a command,and
 * executes it until EOF is recieved or a read error occurs.
 *
 * Return: Always 0
 */
int main(void)
{
	char *line;

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
