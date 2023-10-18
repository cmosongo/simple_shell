#include "main_shell.h"

/**
 * cmp_env_name - compares env variables names
 * with the name passed.
 * @nenv: name of the environment variable
 * @name: name passed
 *
 * Return: 0 if are not equal. Another value if they are.
 */
int cmp_env_name(const char *nenv, const char *name)
{
    int i;

    for (i = 0; nenv[i] != '='; i++)
    {
        if (nenv[i] != name[i])
        {
            return (0);
        }
    }

    return (i + 1);
}

/**
 * _getenv - get an environment variable
 * @name: name of the environment variable
 * @_environ: environment variable
 *
 * Return: value of the environment variable if is found.
 * In other case, returns NULL.
 */
char *_getenv(const char *name, char **_environ)
{
    char *ptr_env;
    int i, mov;

    /* Initialize ptr_env value */
    ptr_env = NULL;
    mov = 0;
    /* Compare all environment variables */
    /* environ is declared in the header file */
    for (i = 0; _environ[i]; i++)
    {
        /* If name and env are equal */
        mov = cmp_env_name(_environ[i], name);
        if (mov)
        {
            ptr_env = _environ[i];
            break;
        }
    }

    return (ptr_env + mov);
}

/**
 * _env - prints the evironment variables
 *
 * @datash: data relevant.
 * Return: 1 on success.
 */
int _env(shell_data_struct *datash)
{
    int i, j;

    for (i = 0; datash->_environ[i]; i++)
    {

        for (j = 0; datash->_environ[i][j]; j++)
            ;

        write(STDOUT_FILENO, datash->_environ[i], j);
        write(STDOUT_FILENO, "\n", 1);
    }
    datash->status = 0;

    return (1);
}

/**
 * copy_info - copies info to create
 * a new env or alias
 * @name: name (env or alias)
 * @value: value (env or alias)
 *
 * Return: new env or alias.
 */
char *copy_info(char *name, char *value)
{
    char *new;
    int len_name, len_value, len;

    len_name = _strlen(name);
    len_value = _strlen(value);
    len = len_name + len_value + 2;
    new = malloc(sizeof(char) * (len));
    _strcpy(new, name);
    _strcat(new, "=");
    _strcat(new, value);
    _strcat(new, "\0");

    return (new);
}

/**
 * set_env - sets an environment variable
 *
 * @name: name of the environment variable
 * @value: value of the environment variable
 * @datash: data structure (environ)
 * Return: no return
 */
void set_env(char *name, char *value, shell_data_struct *datash)
{
    int i;
    char *var_env, *name_env;

    for (i = 0; datash->_environ[i]; i++)
    {
        var_env = _strdup(datash->_environ[i]);
        name_env = _strtok(var_env, "=");
        if (_strcmp(name_env, name) == 0)
        {
            free(datash->_environ[i]);
            datash->_environ[i] = copy_info(name_env, value);
            free(var_env);
            return;
        }
        free(var_env);
    }

    datash->_environ = _reallocdp(datash->_environ, i, sizeof(char *) * (i + 2));
    datash->_environ[i] = copy_info(name, value);
    datash->_environ[i + 1] = NULL;
}
