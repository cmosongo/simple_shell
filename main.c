#include "main.h"

/**
 * display_welcome_message - Run display
 *
 *@void: no parameter
 *
 * Return: welcome message.
 */
void display_welcome_message(void)
{
	printf("Welcome to simple shell\nCTRL+D or type 'exit' to quit\n");
}

/**
 * read_command - Read commands input
 *
 *@void: no parameter
 *
 * Return: Read command.
 */
char *read_command(void)
{
	char *command = NULL;
	size_t command_size = 0;
	ssize_t read_chars;

	printf("[(*°▽°*)] ");

	read_chars = getline(&command, &command_size, stdin);

	if (read_chars == -1)
	{
		if (feof(stdin))
		{
			free(command);
			command = NULL;
		} else
		{
			perror("getline");
		}
	} else
		if (read_chars > 0 && command[read_chars - 1] == '\n')
		{
		command[read_chars - 1] = '\0';
		}

	return (command);
}

/**
 * execute_command - Calls system scripts
 *
 * @command: command to be executed
 *
 * Return: no return
 */
void execute_command(char *command)
{
	pid_t pid;
	int status;

	if (command == NULL)
		return;

	pid = fork();

	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	} else
	if (pid == 0)
	{
		char *args[4];

		args[0] = "/bin/sh";
		args[1] = "-c";
		args[2] = command;
		args[3] = NULL;

		if (execve("/bin/sh", args, NULL) == -1)
		{
			perror("execve");
			_exit(EXIT_FAILURE);
		}
	}
	else
	{
		waitpid(pid, &status, 0);

		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			printf("Command failed to execute.\n");
			/* error handling */
		}
	}

	free(command);
}

/**
 * main - Entry point
 *
 * Return: 0 on success.
 */
int main(void)
{
	bool eof_detected = false;

	display_welcome_message();

	while (!eof_detected)
	{
		char *command = read_command();

		if (command == NULL)
		{
			eof_detected = true;
		continue;
		}

		if (strcmp(command, "exit") == 0)
		{
			printf("Exiting...\n");
			free(command);
			break;
		}

		execute_command(command);
	}

	return (0);
}

