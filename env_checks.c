#include "main_shell.h"

/**
 * _setenv - compares env variables names
 * with the name passed.
 * @datash: data relevant (env name and env value)
 *
 * Return: 1 on success.
 */
int _setenv(shell_data_struct *datash)
{

    if (datash->args[1] == NULL || datash->args[2] == NULL)
    {
        get_error(datash, -1);
        return (1);
    }

    set_env(datash->args[1], datash->args[2], datash);

    return (1);
}

/**
 * _unsetenv - deletes a environment variable
 *
 * @datash: data relevant (env name)
 *
 * Return: 1 on success.
 */
int _unsetenv(shell_data_struct *datash)
{
    char **realloc_environ;
    char *var_env, *name_env;
    int i, j, k;

    if (datash->args[1] == NULL)
    {
        get_error(datash, -1);
        return (1);
    }
    k = -1;
    for (i = 0; datash->_environ[i]; i++)
    {
        var_env = _strdup(datash->_environ[i]);
        name_env = _strtok(var_env, "=");
        if (_strcmp(name_env, datash->args[1]) == 0)
        {
            k = i;
        }
        free(var_env);
    }
    if (k == -1)
    {
        get_error(datash, -1);
        return (1);
    }
    realloc_environ = malloc(sizeof(char *) * (i));
    for (i = j = 0; datash->_environ[i]; i++)
    {
        if (i != k)
        {
            realloc_environ[j] = datash->_environ[i];
            j++;
        }
    }
    realloc_environ[j] = NULL;
    free(datash->_environ[k]);
    free(datash->_environ);
    datash->_environ = realloc_environ;
    return (1);
}

/**
 * strcat_cd - function that concatenates the message for cd error
 *
 * @datash: data relevant (directory)
 * @msg: message to print
 * @error: output message
 * @ver_str: counter lines
 * Return: error message
 */
char *strcat_cd(shell_data_struct *datash, char *msg, char *error, char *ver_str)
{
    char *illegal_flag;

    _strcpy(error, datash->av[0]);
    _strcat(error, ": ");
    _strcat(error, ver_str);
    _strcat(error, ": ");
    _strcat(error, datash->args[0]);
    _strcat(error, msg);
    if (datash->args[1][0] == '-')
    {
        illegal_flag = malloc(3);
        illegal_flag[0] = '-';
        illegal_flag[1] = datash->args[1][1];
        illegal_flag[2] = '\0';
        _strcat(error, illegal_flag);
        free(illegal_flag);
    }
    else
    {
        _strcat(error, datash->args[1]);
    }

    _strcat(error, "\n");
    _strcat(error, "\0");
    return (error);
}

/**
 * error_get_cd - error message for cd command in get_cd
 * @datash: data relevant (directory)
 * Return: Error message
 */
char *error_get_cd(shell_data_struct *datash)
{
    int length, len_id;
    char *error, *ver_str, *msg;

    ver_str = aux_itoa(datash->counter);
    if (datash->args[1][0] == '-')
    {
        msg = ": Illegal option ";
        len_id = 2;
    }
    else
    {
        msg = ": can't cd to ";
        len_id = _strlen(datash->args[1]);
    }

    length = _strlen(datash->av[0]) + _strlen(datash->args[0]);
    length += _strlen(ver_str) + _strlen(msg) + len_id + 5;
    error = malloc(sizeof(char) * (length + 1));

    if (error == 0)
    {
        free(ver_str);
        return (NULL);
    }

    error = strcat_cd(datash, msg, error, ver_str);

    free(ver_str);

    return (error);
}

/**
 * error_not_found - generic error message for command not found
 * @datash: data relevant (counter, arguments)
 * Return: Error message
 */
char *error_not_found(shell_data_struct *datash)
{
    int length;
    char *error;
    char *ver_str;

    ver_str = aux_itoa(datash->counter);
    length = _strlen(datash->av[0]) + _strlen(ver_str);
    length += _strlen(datash->args[0]) + 16;
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
    _strcat(error, ": not found\n");
    _strcat(error, "\0");
    free(ver_str);
    return (error);
}
