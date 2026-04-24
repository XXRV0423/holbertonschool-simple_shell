#include "simple_shell.h"

/**
 * get_path_env - retrieves the PATH value from the environ array
 *
 * Return: pointer to the PATH value string, or NULL if not found
 */
char *get_path_env(void)
{
	int i;

	if (environ == NULL)
		return (NULL);

	for (i = 0; environ[i] != NULL; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
			return (environ[i] + 5);
	}

	return (NULL);
}

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

	if (strchr(cmd, '/') != NULL)
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}

	path_env = get_path_env();
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
		sprintf(full_path, "%s/%s", dir, cmd);

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
 * child_process - runs execve in the child and handles failure
 * @cmd_path: full path to the command
 * @argv: argument vector
 * @prog_name: name of the shell program
 * @cmd_num: command number for error messages
 *
 * Return: void
 */
void child_process(char *cmd_path, char **argv, char *prog_name, int cmd_num)
{
	if (execve(cmd_path, argv, environ) == -1)
	{
		fprintf(stderr, "%s: %d: %s: not found\n",
			prog_name, cmd_num, argv[0]);
		free(cmd_path);
		exit(127);
	}
}

/**
 * execute_command - forks and executes a command
 * @argv: argument vector
 * @prog_name: name of the shell program (argv[0] from main)
 * @cmd_num: command number for error messages
 *
 * Return: exit status of the executed command
 */
int execute_command(char **argv, char *prog_name, int cmd_num)
{
	pid_t pid;
	int status;
	char *cmd_path;

	if (argv[0] == NULL)
		return (0);

	cmd_path = find_in_path(argv[0]);
	if (cmd_path == NULL)
	{
		fprintf(stderr, "%s: %d: %s: not found\n",
			prog_name, cmd_num, argv[0]);
		return (127);
	}

	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0)
		child_process(cmd_path, argv, prog_name, cmd_num);
	else
		waitpid(pid, &status, 0);

	free(cmd_path);

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));

	return (0);
}

/**
 * print_env - prints the current environment
 *
 * Return: void
 */
void print_env(void)
{
	int i = 0;

	while (environ[i] != NULL)
	{
		printf("%s\n", environ[i]);
		i++;
	}
}
