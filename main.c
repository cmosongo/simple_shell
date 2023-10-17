#include "main.h"

int main() {
    char *command = NULL;
    size_t command_size = 0;
    ssize_t read_chars;
    pid_t pid ;
    int status;
    bool eof_detected = false;
	
    printf("Welcome to simple shell v.1.0.0 \nPress 'CTRL +D' or type 'exit' to quit)\n ");
    while (!eof_detected) {
        printf("[(*°▽°*)] ");
        
        /* Read the command*/ 
        read_chars = getline(&command, &command_size, stdin);
        if (read_chars == -1) {
            if (feof(stdin)) {
                eof_detected = true;  /* EOF detected*/ 
            } else {
                perror("getline");
            }
            break;
        }
        
        /*Remove newline character */ 
        if (read_chars > 0 && command[read_chars - 1] == '\n') {
            command[read_chars - 1] = '\0';
        }

        /* Check if the user wants to exit */ 
        if (strcmp(command, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }

        /* Fork a new process*/ 
        pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            /*Child process: execute the command using execve */ 
            char *args[4];
            args[0] = "/bin/sh";
            args[1] = "-c";
            args[2] = command;
            args[3] = NULL;

            if (execve("/bin/sh", args, NULL) == -1) {
                perror("execve");
                _exit(EXIT_FAILURE);
            }
        } else {
            /* Parent process: wait for the child to complete*/ 
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                printf("Command failed to execute.\n");
                /*error handling*/
            } 
            
	    /*
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                printf("Command executed successfully.\n");
            } else {
                printf("Command failed to execute.\n");
            }
            */
        }
    }

    /* Free dynamically allocated memory*/ 
    free(command);

    return 0;
}

