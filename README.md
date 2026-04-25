# Simple Shell

A simple UNIX command-line interpreter written in C, built as the final project of the first trimester at **Holberton School**. This shell replicates core behaviors of `/bin/sh`, including prompt display, command parsing, PATH resolution, process forking, and built-in command handling.

---

## Table of Contents

- [About the Project](#about-the-project)
- [Features](#features)
- [File Structure](#file-structure)
- [Function Reference](#function-reference)
  - [shell.c](#shellc)
  - [executor.c](#executorc)
  - [simple_shell.h](#simple_shellh)
- [Built-in Commands](#built-in-commands)
- [How It Works](#how-it-works)
- [Compilation](#compilation)
- [Usage](#usage)
  - [Interactive Mode](#interactive-mode)
  - [Non-Interactive Mode](#non-interactive-mode)
- [Examples](#examples)
- [Requirements](#requirements)
- [Authors](#authors)

---

## About the Project

This project is a simple shell written in C that mimics the behavior of a standard UNIX shell. It was developed as the culminating project of the first trimester at Holberton School, applying concepts such as system calls, process management, memory allocation, and string manipulation.

The shell reads commands from the user, searches for them in the system's `PATH`, and executes them as child processes using `fork()` and `execve()`.

---

## Features

- Displays an interactive prompt (`$ `) when connected to a terminal
- Reads and parses user input line by line
- Searches for commands across all directories listed in the `PATH` environment variable
- Supports commands with arguments
- Handles absolute and relative paths to executables
- Forks child processes to execute commands without blocking the shell
- Tracks command numbers for accurate error reporting
- Built-in `exit` command to terminate the shell
- Built-in `env` command to print environment variables
- Handles `EOF` (end-of-file / `Ctrl+D`) gracefully
- Supports both interactive and non-interactive modes

---

## File Structure

```
simple_shell/
├── simple_shell.h   # Header file: includes, macros, prototypes
├── shell.c          # Core shell loop: prompt, input reading, tokenizing, main
└── executor.c       # Command resolution and execution logic
```

---

## Function Reference

### `shell.c`

| Function | Description |
|---|---|
| `display_prompt()` | Prints the `$ ` prompt to stdout only when connected to a terminal (interactive mode). Uses `isatty()` to detect terminal. |
| `read_line()` | Reads one line of input from `stdin` using `getline()`. Returns `NULL` on `EOF` and handles the newline print for interactive mode. |
| `split_line(char *line)` | Tokenizes the input line using `strtok()` with delimiters (space, tab, carriage return, newline). Returns a dynamically allocated array of token strings. Dynamically grows the buffer if needed. |
| `main(int argc, char **argv)` | Entry point of the shell. Runs the main REPL loop: display prompt → read line → tokenize → handle built-ins → execute command → repeat. Tracks the last exit status and command number. |

### `executor.c`

| Function | Description |
|---|---|
| `get_path_env()` | Iterates over the `environ` array to find and return the value of the `PATH` variable (the part after `PATH=`). |
| `find_in_path(char *cmd)` | Searches each directory in `PATH` for an executable matching `cmd`. If `cmd` contains a `/`, it checks directly via `access()`. Returns the full path string if found, or `NULL`. |
| `child_process(char *cmd_path, char **argv, char *prog_name, int cmd_num)` | Called inside the child process after `fork()`. Executes the command using `execve()`. If execution fails, prints an error to `stderr` and exits with code `127`. |
| `execute_command(char **argv, char *prog_name, int cmd_num)` | Resolves the command path with `find_in_path()`, forks a child process, calls `child_process()` in the child, and waits for it to finish in the parent. Returns the exit status. |
| `print_env()` | Iterates over the `environ` array and prints each environment variable to `stdout`. |

### `simple_shell.h`

The header file that ties the project together. It contains:

- **Standard library includes:** `<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<unistd.h>`, `<sys/types.h>`, `<sys/wait.h>`
- **Macros:**
  - `PROMPT` — The shell prompt string (`"$ "`)
  - `TOK_DELIM` — Delimiter characters used for tokenizing (`" \t\r\n"`)
  - `TOK_BUFSIZE` — Initial and growth size for the token buffer (`64`)
- **`extern char **environ`** — Declaration of the external environment variable array
- **Function prototypes** for all functions across `shell.c` and `executor.c`

---

## Built-in Commands

| Command | Description |
|---|---|
| `exit` | Exits the shell, returning the last command's exit status |
| `env` | Prints all current environment variables to stdout |

---

## How It Works

The shell follows a standard **Read → Evaluate → Print Loop (REPL)**:

```
┌─────────────────────────────────────────────┐
│                   START                     │
└─────────────────────┬───────────────────────┘
                      │
                      ▼
             display_prompt()
                      │
                      ▼
              read_line()  ──── EOF ──▶ EXIT
                      │
                      ▼
             split_line(line)
                      │
                      ▼
            ┌─────────────────┐
            │  Built-in cmd?  │
            │  exit / env     │
            └────────┬────────┘
               yes   │    no
               ▼     │     ▼
            handle   │  execute_command()
                     │       │
                     │       ▼
                     │  find_in_path()
                     │       │
                     │       ▼
                     │    fork()
                     │   /      \
                     │child     parent
                     │  │         │
                     │execve()  waitpid()
                     │          │
                     └──────────┘
                           │
                           ▼
                      (loop back)
```

1. The shell displays a prompt and waits for user input.
2. The input line is read and split into tokens (command + arguments).
3. Built-in commands (`exit`, `env`) are handled directly without forking.
4. For external commands, the shell searches the `PATH` environment variable to find the executable.
5. A child process is forked. The child calls `execve()` to replace itself with the command.
6. The parent waits for the child to finish and captures its exit status.
7. The loop repeats until `EOF` or `exit` is called.

---

## Compilation

Compile using `gcc` with the following flags:

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

---

## Usage

### Interactive Mode

Run the shell directly to enter interactive mode. The prompt `$ ` will appear and wait for input:

```bash
$ ./hsh
$ ls -l
$ echo Hello, Holberton!
$ exit
```

### Non-Interactive Mode

Pipe commands directly into the shell for scripted or automated use:

```bash
echo "ls -l" | ./hsh
echo "/bin/echo Hello" | ./hsh
```

---

## Examples

```bash
$ ./hsh
$ echo Hello, World!
Hello, World!
$ ls
executor.c  hsh  README.md  shell.c  simple_shell.h
$ /bin/pwd
/home/user/simple_shell
$ env
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
...
$ unknowncmd
./hsh: 5: unknowncmd: not found
$ exit
```

**Non-interactive example:**
```bash
$ echo "echo Holberton School" | ./hsh
Holberton School
```

---

## Requirements

- **OS:** Ubuntu 20.04 LTS (or compatible Linux distribution)
- **Compiler:** GCC
- **Standard:** `gnu89`
- **No use of:** `system()`, `execl()`, `execlp()`, `execle()`, `execvp()`, or `execvpe()`

---

## Authors


| Name | GitHub |
|---|---|
| Xander X. Roldan | [@XXRV0423](https://github.com/XXRV0423) |
| Alejandro J. Colombani | [@alejandrojcolombani](https://github.com/alejandrojcolombani) |
