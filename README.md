# SIMPLE SHELL 

## Running the shell

- **Clone the repository**

Clone this repository into your working directory. 

`git clone https://github.com/cmosongo/simple_shell.git`

- **Compile**
Files should be compiled with GCC and the 
following flags: -Wall -Wextra -Werror -pedantic -std=gnu89.

`simple_shell$   gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh`

- **Launch**

**Interactive Mode**
Runs in interactive or non-interactive mode.
Interactive Mode

`simple_shell$ ./hsh`

Run the program and wait for the prompt to appear. From there, you can type commands and when done exit with ctrl+D or exit.

`[(*°▽°*)] exit`


**Non-interactive Mode**

Echo your desired command and pipe it into the program like this:

`simple_shell$ echo "ls" | ./hsh`

This file contains the details of all the individuals that contributed to this shell project.
Features

1. Display a prompt and wait for the user to type a command. A command line always ends with a new line. 
2. The prompt is displayed again each time a command has been executed. 
3. The command lines are simple, no semicolons, no pipes, no redirections or any other advanced features. 
4. The command lines are made only of one word. No arguments will be passed to programs. 
5. If an executable cannot be found, prints an error message and display the prompt again.
