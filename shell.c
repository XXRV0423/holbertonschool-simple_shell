#include "simple_shell.h"

/**
 * display_prompt - Prints the shell prompt to stdout
 * Only displays when stdin is connected to a terminal
 *
 * Return: void
 */
void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, PROMPT, 2);
}
