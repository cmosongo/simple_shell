#include "main_shell.h"

/**
 * main - Entry point
 *
 * @ac: argument count
 * @av: argument vector
 *
 * Return: 0 on success.
 */
int main(int ac, char **av)
{
    shell_data_struct datash;
    (void)ac;

    signal(SIGINT, get_sigint);
    set_data(&datash, av);
    shell_loop(&datash);
    free_data(&datash);
    if (datash.status < 0)
        return (255);
    return (datash.status);
}
