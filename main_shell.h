#ifndef _MAIN_SHELL_
#define _MAIN_SHELL_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>

#define BUFFER_SIZE 1024
#define SIZE_TOKEN_BUFFER 128
#define TOKEN_DELIMINATOR " \t\r\n\a"

/* Points to an array of pointers to strings called the "environment" */
extern char **environ;

/**
 * struct data - struct that contains all relevant data on runtime
 * @av: argument vector
 * @input: command line written by the user
 * @args: tokens of the command line
 * @status: last status of the shell
 * @counter: lines counter
 * @_environ: environment variable
 * @pid: process ID of the shell
 */
typedef struct data
{
	char **av;
	char *input;
	char **args;
	int status;
	int counter;
	char **_environ;
	char *pid;
} shell_data_struct;

/**
 * struct sep_list_s - single linked list
 * @separator: ; | &
 * @next: next node
 * Description: single linked list to store separators
 */
typedef struct sep_list_s
{
	char separator;
	struct sep_list_s *next;
} sep_list;

/**
 * struct line_list_s - single linked list
 * @line: command line
 * @next: next node
 * Description: single linked list to store command lines
 */
typedef struct line_list_s
{
	char *line;
	struct line_list_s *next;
} line_list;

/**
 * struct r_var_list - single linked list
 * @len_var: length of the variable
 * @val: value of the variable
 * @len_val: length of the value
 * @next: next node
 * Description: single linked list to store variables
 */
typedef struct r_var_list
{
	int len_var;
	char *val;
	int len_val;
	struct r_var_list *next;
} r_var;

/**
 * struct builtin_s - Builtin struct for command args.
 * @name: The name of the command builtin i.e cd, exit, env
 * @f: data type pointer function.
 */
typedef struct builtin_s
{
	char *name;
	int (*f)(shell_data_struct *datash);
} builtin_t;

/* aux_helpers1.c */
void func_aux_help_env(void);
void func_aux_help_set_env(void);
void func_aux_help_unset_env(void);
void aux_help_general(void);

/* aux_helpers2.c */
void aux_help_exit(void);
void aux_help(void);
void aux_help_alias(void);
void aux_help_cd(void);

/* aux_lists_func1.c */
sep_list *add_sep_node_end(sep_list **head, char sep);
void free_sep_list(sep_list **head);
line_list *add_line_node_end(line_list **head, char *line);
void free_line_list(line_list **head);

/* aux_lists_func2.c */
r_var *add_rvar_node(r_var **head, int lvar, char *var, int lval);
void free_rvar_list(r_var **head);

/* str_func_1.c */
char *_strcat(char *dest, const char *src);
char *_strcpy(char *dest, char *src);
int _strcmp(char *s1, char *s2);
char *_strchr(char *s, char c);
int _strspn(char *s, char *accept);

/* str_func_2.c */
char *_strdup(const char *s);
int _strlen(const char *s);
int cmp_chars(char str[], const char *delim);
char *_strtok(char str[], const char *delim);
int _isdigit(const char *s);

/* str_func_3.c */
void rev_string(char *s);

/* mem_funcs.c */
void _memcpy(void *newptr, const void *ptr, unsigned int size);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
char **_reallocdp(char **ptr, unsigned int old_size, unsigned int new_size);

/* syntax_error_check.c */
int repeated_char(char *input, int i);
int error_sep_op(char *input, int i, char last);
int first_char(char *input, int *i);
void print_syntax_error(shell_data_struct *datash, char *input, int i, int bool);
int check_syntax_error(shell_data_struct *datash, char *input);

/* shell_funcs.c */
char *without_comment(char *in);
void shell_loop(shell_data_struct *datash);

/* line_funcs.c */
char *read_line(int *i_eof);
void bring_line(char **lineptr, size_t *n, char *buffer, size_t j);
ssize_t get_line(char **lineptr, size_t *n, FILE *stream);
int exec_line(shell_data_struct *datash);

/* split_func.c */
char *swap_char(char *input, int bool);
void add_nodes(sep_list **head_s, line_list **head_l, char *input);
void go_next(sep_list **list_s, line_list **list_l, shell_data_struct *datash);
int split_commands(shell_data_struct *datash, char *input);
char **split_line(char *input);

/* var_checks.c */
void check_env(r_var **h, char *in, shell_data_struct *data);
int check_vars(r_var **h, char *in, char *st, shell_data_struct *data);
char *replaced_input(r_var **head, char *input, char *new_input, int nlen);
char *rep_var(char *input, shell_data_struct *datash);

/* cmd_calls.c */
int is_cdir(char *path, int *i);
char *_which(char *cmd, char **_environ);
int is_executable(shell_data_struct *datash);
int check_error_cmd(char *dir, shell_data_struct *datash);
int cmd_exec(shell_data_struct *datash);

/* env_checks.c */
char *_getenv(const char *name, char **_environ);
int _env(shell_data_struct *datash);

/* env_checks2.c */
char *copy_info(char *name, char *value);
void set_env(char *name, char *value, shell_data_struct *datash);
int _setenv(shell_data_struct *datash);
int _unsetenv(shell_data_struct *datash);

/* cd_call.c */
void cd_dot(shell_data_struct *datash);
void cd_to(shell_data_struct *datash);
void cd_previous(shell_data_struct *datash);
void cd_to_home(shell_data_struct *datash);
int cd_shell(shell_data_struct *datash);

/* _exit.c */
int exit_shell(shell_data_struct *datash);

/* aux_stdlib.c */
int get_len(int n);
char *aux_itoa(int n);
int _atoi(char *s);

/* error_checks1.c */
char *strcat_cd(shell_data_struct *, char *, char *, char *);
char *error_get_cd(shell_data_struct *datash);
char *error_not_found(shell_data_struct *datash);
char *error_exit_shell(shell_data_struct *datash);

/* error_checks2.c */
char *error_get_alias(char **args);
char *error_env(shell_data_struct *datash);
char *error_syntax(char **args);
char *error_permission(char **args);
char *error_path_126(shell_data_struct *datash);

/* get_funcs1.c */
int get_error(shell_data_struct *datash, int eval);
void get_sigint(int sig);
int get_help(shell_data_struct *datash);
int (*get_builtin(char *cmd))(shell_data_struct *datash);

/* main_calls.c */
void free_data(shell_data_struct *datash);
void set_data(shell_data_struct *datash, char **av);

#endif
