#include "main_shell.h"

/**
 * is_executable - determines if is an executable
 *
 * @datash: data structure
 * Return: 0 if is not an executable, other number if it does
 */
int is_executable(shell_data_struct *datash)
{
    struct stat st;
    int i;
    char *input;

    input = datash->args[0];
    for (i = 0; input[i]; i++)
    {
        if (input[i] == '.')
        {
            if (input[i + 1] == '.')
                return (0);
            if (input[i + 1] == '/')
                continue;
            else
                break;
        }
        else if (input[i] == '/' && i != 0)
        {
            if (input[i + 1] == '.')
                continue;
            i++;
            break;
        }
        else
            break;
    }
    if (i == 0)
        return (0);

    if (stat(input + i, &st) == 0)
    {
        return (i);
    }
    get_error(datash, 127);
    return (-1);
}

/**
 * check_error_cmd - verifies if user has permissions to access
 *
 * @dir: destination directory
 * @datash: data structure
 * Return: 1 if there is an error, 0 if not
 */
int check_error_cmd(char *dir, shell_data_struct *datash)
{
    if (dir == NULL)
    {
        get_error(datash, 127);
        return (1);
    }

    if (_strcmp(datash->args[0], dir) != 0)
    {
        if (access(dir, X_OK) == -1)
        {
            get_error(datash, 126);
            free(dir);
            return (1);
        }
        free(dir);
    }
    else
    {
        if (access(datash->args[0], X_OK) == -1)
        {
            get_error(datash, 126);
            return (1);
        }
    }

    return (0);
}

/**
 * cmd_exec - executes command lines
 *
 * @datash: data relevant (args and input)
 * Return: 1 on success.
 */
int cmd_exec(shell_data_struct *datash)
{
    pid_t pd;
    pid_t wpd;
    int state;
    int exec;
    char *dir;
    (void)wpd;

    exec = is_executable(datash);
    if (exec == -1)
        return (1);
    if (exec == 0)
    {
        dir = _which(datash->args[0], datash->_environ);
        if (check_error_cmd(dir, datash) == 1)
            return (1);
    }

    pd = fork();
    if (pd == 0)
    {
        if (exec == 0)
            dir = _which(datash->args[0], datash->_environ);
        else
            dir = datash->args[0];
        execve(dir + exec, datash->args, datash->_environ);
    }
    else if (pd < 0)
    {
        perror(datash->av[0]);
        return (1);
    }
    else
    {
        do
        {
            wpd = waitpid(pd, &state, WUNTRACED);
        } while (!WIFEXITED(state) && !WIFSIGNALED(state));
    }

    datash->status = state / 256;
    return (1);
}

/**
 * exec_line - finds builtins and commands
 *
 * @datash: data relevant (args)
 * Return: 1 on success.
 */
int exec_line(shell_data_struct *datash)
{
    int (*builtin)(shell_data_struct *datash);

    if (datash->args[0] == NULL)
        return (1);

    builtin = get_builtin(datash->args[0]);

    if (builtin != NULL)
        return (builtin(datash));

    return (cmd_exec(datash));
}

/**
 * exit_shell - exits the shell
 *
 * @datash: data relevant (status and args)
 * Return: 0 on success.
 */
int exit_shell(shell_data_struct *datash)
{
    unsigned int ustatus;
    int is_digit;
    int str_len;
    int big_number;

    if (datash->args[1] != NULL)
    {
        ustatus = _atoi(datash->args[1]);
        is_digit = _isdigit(datash->args[1]);
        str_len = _strlen(datash->args[1]);
        big_number = ustatus > (unsigned int)INT_MAX;
        if (!is_digit || str_len > 10 || big_number)
        {
            get_error(datash, 2);
            datash->status = 2;
            return (1);
        }
        datash->status = (ustatus % 256);
    }
    return (0);
}
