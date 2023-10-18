#include "main_shell.h"

/**
 * error_exit_shell - generic error message for exit in get_exit
 * @datash: data relevant (counter, arguments)
 *
 * Return: Error message
 */
char *error_exit_shell(shell_data_struct *datash)
{
    int length;
    char *error;
    char *ver_str;

    ver_str = aux_itoa(datash->counter);
    length = _strlen(datash->av[0]) + _strlen(ver_str);
    length += _strlen(datash->args[0]) + _strlen(datash->args[1]) + 23;
    error = malloc(sizeof(char) * (length + 1));
    if (error == 0)
    {
        free(ver_str);
        return (NULL);
    }
    _strcpy(error, datash->av[0]);
    _strcat(error, ": ");
    _strcat(error, ver_str);
    _strcat(error, ": ");
    _strcat(error, datash->args[0]);
    _strcat(error, ": Illegal number: ");
    _strcat(error, datash->args[1]);
    _strcat(error, "\n\0");
    free(ver_str);

    return (error);
}

/**
 * error_env - error message for env in get_env.
 * @datash: data relevant (counter, arguments)
 * Return: error message.
 */
char *error_env(shell_data_struct *datash)
{
    int length;
    char *error;
    char *ver_str;
    char *msg;

    ver_str = aux_itoa(datash->counter);
    msg = ": Unable to add/remove from environment\n";
    length = _strlen(datash->av[0]) + _strlen(ver_str);
    length += _strlen(datash->args[0]) + _strlen(msg) + 4;
    error = malloc(sizeof(char) * (length + 1));
    if (error == 0)
    {
        free(error);
        free(ver_str);
        return (NULL);
    }

    _strcpy(error, datash->av[0]);
    _strcat(error, ": ");
    _strcat(error, ver_str);
    _strcat(error, ": ");
    _strcat(error, datash->args[0]);
    _strcat(error, msg);
    _strcat(error, "\0");
    free(ver_str);

    return (error);
}

/**
 * error_path_126 - error message for path and failure denied permission.
 * @datash: data relevant (counter, arguments).
 *
 * Return: The error string.
 */
char *error_path_126(shell_data_struct *datash)
{
    int length;
    char *ver_str;
    char *error;

    ver_str = aux_itoa(datash->counter);
    length = _strlen(datash->av[0]) + _strlen(ver_str);
    length += _strlen(datash->args[0]) + 24;
    error = malloc(sizeof(char) * (length + 1));
    if (error == 0)
    {
        free(error);
        free(ver_str);
        return (NULL);
    }
    _strcpy(error, datash->av[0]);
    _strcat(error, ": ");
    _strcat(error, ver_str);
    _strcat(error, ": ");
    _strcat(error, datash->args[0]);
    _strcat(error, ": Permission denied\n");
    _strcat(error, "\0");
    free(ver_str);
    return (error);
}

/**
 * is_cdir - checks ":" if is in the current directory.
 * @path: type char pointer char.
 * @i: type int pointer of index.
 * Return: 1 if the path is searchable in the cd, 0 otherwise.
 */
int is_cdir(char *path, int *i)
{
    if (path[*i] == ':')
        return (1);

    while (path[*i] != ':' && path[*i])
    {
        *i += 1;
    }

    if (path[*i])
        *i += 1;

    return (0);
}

/**
 * _which - locates a command
 *
 * @cmd: command name
 * @_environ: environment variable
 * Return: location of the command.
 */
char *_which(char *cmd, char **_environ)
{
    char *path, *ptr_path, *token_path, *dir;
    int len_dir, len_cmd, i;
    struct stat st;

    path = _getenv("PATH", _environ);
    if (path)
    {
        ptr_path = _strdup(path);
        len_cmd = _strlen(cmd);
        token_path = _strtok(ptr_path, ":");
        i = 0;
        while (token_path != NULL)
        {
            if (is_cdir(path, &i))
                if (stat(cmd, &st) == 0)
                    return (cmd);
            len_dir = _strlen(token_path);
            dir = malloc(len_dir + len_cmd + 2);
            _strcpy(dir, token_path);
            _strcat(dir, "/");
            _strcat(dir, cmd);
            _strcat(dir, "\0");
            if (stat(dir, &st) == 0)
            {
                free(ptr_path);
                return (dir);
            }
            free(dir);
            token_path = _strtok(NULL, ":");
        }
        free(ptr_path);
        if (stat(cmd, &st) == 0)
            return (cmd);
        return (NULL);
    }
    if (cmd[0] == '/')
        if (stat(cmd, &st) == 0)
            return (cmd);
    return (NULL);
}
