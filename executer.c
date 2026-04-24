#include "simple_shell.h"

/**
 * find_in_path - searches for a command in the PATH directories
 * @cmd: command name to search
 *
 * Return: full path to the command if found, NULL otherwise
 */
char *find_in_path(char *cmd)
{
	char *path_env, *path_copy, *dir, *full_path;
	int len;

	if (cmd == NULL)
		return (NULL);

	if (access(cmd, X_OK) == 0)
		return (strdup(cmd));

	path_env = getenv("PATH");
	if (path_env == NULL || *path_env == '\0')
		return (NULL);

	path_copy = strdup(path_env);
	if (path_copy == NULL)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		len = strlen(dir) + strlen(cmd) + 2;
		full_path = malloc(len);
		if (full_path == NULL)
		{
			free(path_copy);
			return (NULL);
		}
		sprintf(full_path, "%s/%S", dir, cmd);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
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
	char *cmd_path;

	if (argv[0] == NULL)
		return;

	cmd_path = find_in_path(argv[0]);
	if (cmd_path == NULL)
	{
		fprintf(stderr, "./simple_shell: 1: %s: not found\n", argv[0]);
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0)
	{
		if (execve(cmd_path, argv, environ) == -1)
		{
			fprintf(stderr, "./simple_shell: 1: %s: not found\n", argv[0]);
			free(cmd_path);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
	}
	free(cmd_path);
}
