#include "main_shell.h"

/**
 * _memcpy - copies information between void pointers.
 * @newptr: destination pointer.
 * @ptr: source pointer.
 * @size: size of the new pointer.
 *
 * Return: no return.
 */
void _memcpy(void *newptr, const void *ptr, unsigned int size)
{
    char *char_ptr = (char *)ptr;
    char *char_newptr = (char *)newptr;
    unsigned int i;

    i = 0;
    while (i < size)
    {
        char_newptr[i] = char_ptr[i];
        i++;
    }
}

/**
 * _realloc - reallocates a memory block.
 * @ptr: pointer to the memory previously allocated.
 * @old_size: size, in bytes, of the allocated space of ptr.
 * @new_size: new size, in bytes, of the new memory block.
 *
 * Return: ptr.
 * if new_size == old_size, returns ptr without changes.
 * if malloc fails, returns NULL.
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
    void *newptr;

    if (ptr == NULL)
        return (malloc(new_size));

    if (new_size == 0)
    {
        free(ptr);
        return (NULL);
    }

    if (new_size == old_size)
        return (ptr);

    newptr = malloc(new_size);
    if (newptr == NULL)
        return (NULL);

    if (new_size < old_size)
        _memcpy(newptr, ptr, new_size);
    else
        _memcpy(newptr, ptr, old_size);

    free(ptr);
    return (newptr);
}

/**
 * _reallocdp - reallocates a memory block of a double pointer.
 * @ptr: double pointer to the memory previously allocated.
 * @old_size: size, in bytes, of the allocated space of ptr.
 * @new_size: new size, in bytes, of the new memory block.
 *
 * Return: ptr.
 * if new_size == old_size, returns ptr without changes.
 * if malloc fails, returns NULL.
 */
char **_reallocdp(char **ptr, unsigned int old_size, unsigned int new_size)
{
    char **newptr;
    unsigned int i;

    if (ptr == NULL)
        return (malloc(sizeof(char *) * new_size));

    if (new_size == old_size)
        return (ptr);

    newptr = malloc(sizeof(char *) * new_size);
    if (newptr == NULL)
        return (NULL);

    for (i = 0; i < old_size; i++)
        newptr[i] = ptr[i];

    free(ptr);

    return (newptr);
}

/**
 * get_builtin - builtin that pais the command in the arg
 * @cmd: command
 * Return: function pointer of the builtin command
 */
int (*get_builtin(char *cmd))(shell_data_struct *)
{
    builtin_t builtin[] = {
        {"env", _env},
        {"exit", exit_shell},
        {"setenv", _setenv},
        {"unsetenv", _unsetenv},
        {"cd", cd_shell},
        {"help", get_help},
        {NULL, NULL}};
    int i;

    for (i = 0; builtin[i].name; i++)
    {
        if (_strcmp(builtin[i].name, cmd) == 0)
            break;
    }

    return (builtin[i].f);
}