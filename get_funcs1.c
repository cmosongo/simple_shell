#include "main_shell.h"

/**
 * split_commands - splits command lines according to
 * the separators ;, | and &, and executes them
 *
 * @datash: data structure
 * @input: input string
 * Return: 0 to exit, 1 to continue
 */
int split_commands(shell_data_struct *datash, char *input)
{

    sep_list *head_s, *list_s;
    line_list *head_l, *list_l;
    int loop;

    head_s = NULL;
    head_l = NULL;

    add_nodes(&head_s, &head_l, input);

    list_s = head_s;
    list_l = head_l;

    while (list_l != NULL)
    {
        datash->input = list_l->line;
        datash->args = split_line(datash->input);
        loop = exec_line(datash);
        free(datash->args);

        if (loop == 0)
            break;

        go_next(&list_s, &list_l, datash);

        if (list_l != NULL)
            list_l = list_l->next;
    }

    free_sep_list(&head_s);
    free_line_list(&head_l);

    if (loop == 0)
        return (0);
    return (1);
}

/**
 * split_line - tokenizes the input string
 *
 * @input: input string.
 * Return: string splitted.
 */
char **split_line(char *input)
{
    size_t bsize;
    size_t i;
    char **tokens;
    char *token;

    bsize = SIZE_TOKEN_BUFFER;
    tokens = malloc(sizeof(char *) * (bsize));
    if (tokens == NULL)
    {
        write(STDERR_FILENO, ": allocation error\n", 18);
        exit(EXIT_FAILURE);
    }

    token = _strtok(input, TOKEN_DELIMINATOR);
    tokens[0] = token;

    for (i = 1; token != NULL; i++)
    {
        if (i == bsize)
        {
            bsize += SIZE_TOKEN_BUFFER;
            tokens = _reallocdp(tokens, i, sizeof(char *) * bsize);
            if (tokens == NULL)
            {
                write(STDERR_FILENO, ": allocation error\n", 18);
                exit(EXIT_FAILURE);
            }
        }
        token = _strtok(NULL, TOKEN_DELIMINATOR);
        tokens[i] = token;
    }

    return (tokens);
}

/**
 * get_len - Get the lenght of a number.
 * @n: type int number.
 * Return: Lenght of a number.
 */
int get_len(int n)
{
    unsigned int n1;
    int lenght = 1;

    if (n < 0)
    {
        lenght++;
        n1 = n * -1;
    }
    else
    {
        n1 = n;
    }
    while (n1 > 9)
    {
        lenght++;
        n1 = n1 / 10;
    }

    return (lenght);
}
/**
 * aux_itoa - function converts int to string.
 * @n: type int number
 * Return: String.
 */
char *aux_itoa(int n)
{
    unsigned int n1;
    int lenght = get_len(n);
    char *buffer;

    buffer = malloc(sizeof(char) * (lenght + 1));
    if (buffer == 0)
        return (NULL);

    *(buffer + lenght) = '\0';

    if (n < 0)
    {
        n1 = n * -1;
        buffer[0] = '-';
    }
    else
    {
        n1 = n;
    }

    lenght--;
    do
    {
        *(buffer + lenght) = (n1 % 10) + '0';
        n1 = n1 / 10;
        lenght--;
    } while (n1 > 0);
    return (buffer);
}

/**
 * _atoi - converts a string to an integer.
 * @s: input string.
 * Return: integer.
 */
int _atoi(char *s)
{
    unsigned int count = 0, size = 0, oi = 0, pn = 1, m = 1, i;

    while (*(s + count) != '\0')
    {
        if (size > 0 && (*(s + count) < '0' || *(s + count) > '9'))
            break;

        if (*(s + count) == '-')
            pn *= -1;

        if ((*(s + count) >= '0') && (*(s + count) <= '9'))
        {
            if (size > 0)
                m *= 10;
            size++;
        }
        count++;
    }

    for (i = count - size; i < count; i++)
    {
        oi = oi + ((*(s + i) - 48) * m);
        m /= 10;
    }
    return (oi * pn);
}
