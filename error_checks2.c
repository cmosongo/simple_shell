#include "main_shell.h"

/**
 * check_syntax_error - intermediate function to
 * find and print a syntax error
 *
 * @datash: data structure
 * @input: input string
 * Return: 1 if there is an error. 0 in other case
 */
int check_syntax_error(shell_data_struct *datash, char *input)
{
    int begin = 0;
    int f_char = 0;
    int i = 0;

    f_char = first_char(input, &begin);
    if (f_char == -1)
    {
        print_syntax_error(datash, input, begin, 0);
        return (1);
    }

    i = error_sep_op(input + begin, 0, *(input + begin));
    if (i != 0)
    {
        print_syntax_error(datash, input, begin + i, 1);
        return (1);
    }

    return (0);
}

/**
 * shell_loop - Loop of shell
 * @datash: data relevant (av, input, args)
 *
 * Return: no return.
 */
void shell_loop(shell_data_struct *datash)
{
    int loop, i_eof;
    char *input;

    loop = 1;
    while (loop == 1)
    {
        write(STDIN_FILENO, "(U&I) ", 5);
        input = read_line(&i_eof);
        if (i_eof != -1)
        {
            input = without_comment(input);
            if (input == NULL)
                continue;

            if (check_syntax_error(datash, input) == 1)
            {
                datash->status = 2;
                free(input);
                continue;
            }
            input = rep_var(input, datash);
            loop = split_commands(datash, input);
            datash->counter += 1;
            free(input);
        }
        else
        {
            loop = 0;
            free(input);
        }
    }
}

/**
 * swap_char - swaps | and & for non-printed chars
 *
 * @input: input string
 * @bool: type of swap
 * Return: swapped string
 */
char *swap_char(char *input, int bool)
{
    int i;

    if (bool == 0)
    {
        for (i = 0; input[i]; i++)
        {
            if (input[i] == '|')
            {
                if (input[i + 1] != '|')
                    input[i] = 16;
                else
                    i++;
            }

            if (input[i] == '&')
            {
                if (input[i + 1] != '&')
                    input[i] = 12;
                else
                    i++;
            }
        }
    }
    else
    {
        for (i = 0; input[i]; i++)
        {
            input[i] = (input[i] == 16 ? '|' : input[i]);
            input[i] = (input[i] == 12 ? '&' : input[i]);
        }
    }
    return (input);
}

/**
 * add_nodes - add separators and command lines in the lists
 *
 * @head_s: head of separator list
 * @head_l: head of command lines list
 * @input: input string
 * Return: no return
 */
void add_nodes(sep_list **head_s, line_list **head_l, char *input)
{
    int i;
    char *line;

    input = swap_char(input, 0);

    for (i = 0; input[i]; i++)
    {
        if (input[i] == ';')
            add_sep_node_end(head_s, input[i]);

        if (input[i] == '|' || input[i] == '&')
        {
            add_sep_node_end(head_s, input[i]);
            i++;
        }
    }

    line = _strtok(input, ";|&");
    do
    {
        line = swap_char(line, 1);
        add_line_node_end(head_l, line);
        line = _strtok(NULL, ";|&");
    } while (line != NULL);
}

/**
 * go_next - go to the next command line stored
 *
 * @list_s: separator list
 * @list_l: command line list
 * @datash: data structure
 * Return: no return
 */
void go_next(sep_list **list_s, line_list **list_l, shell_data_struct *datash)
{
    int loop_sep;
    sep_list *ls_s;
    line_list *ls_l;

    loop_sep = 1;
    ls_s = *list_s;
    ls_l = *list_l;

    while (ls_s != NULL && loop_sep)
    {
        if (datash->status == 0)
        {
            if (ls_s->separator == '&' || ls_s->separator == ';')
                loop_sep = 0;
            if (ls_s->separator == '|')
                ls_l = ls_l->next, ls_s = ls_s->next;
        }
        else
        {
            if (ls_s->separator == '|' || ls_s->separator == ';')
                loop_sep = 0;
            if (ls_s->separator == '&')
                ls_l = ls_l->next, ls_s = ls_s->next;
        }
        if (ls_s != NULL && !loop_sep)
            ls_s = ls_s->next;
    }

    *list_s = ls_s;
    *list_l = ls_l;
}
