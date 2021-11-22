#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "run.h"

/**
 * Determines if the given argument is a shell command
 *
 * @param args Array of arguments
 * @return 1 if true, 0 if false
 */
int is_shell_cmd(char **args) {
    char *name = args[0];

    if (strcmp(name, "exit") == 0) {
        return 1;
    }
    else if (strcmp(name, "cd") == 0) {
        return 1;
    }
    else return 0;
}

/**
 * Takes in a shell command and executes them
 *
 * @param args Array of arguments
 * @return 0 on success and other values on failure
 */
int do_shell_cmd(char **args) {
    char *name = args[0];

    if (strcmp(name, "exit") == 0) {
        exit(0);
    }

    else if (strcmp(name, "cd") == 0) {
        int result = chdir(args[1]);
        if (result) {
            printf("dash: %s\n", strerror(errno));
            return result;
        }
        return 0;
    }

    return 1;
}

/**
 * Takes in a regular command and executes it
 *
 * @param args Array of arguments
 * @return 0 on success and other values on failure
 */
int run_command(char **args) {
    int f = fork();

    if (!f) {
        if (execvp(args[0], args)) {
        	printf("dash: %s\n", strerror(errno));
        }
        exit(0);
    } else {
        int status;
        int pid = wait(&status);
    }
}

void run_tests() {
    printf("\n\nrun.c Tests:\n");

    char *exit_args[] = {"exit"};
    printf("exit: %d\n", is_shell_cmd(exit_args));

    char *cd_args[] = {"cd", ".."};
    printf("cd ..: %d\n", is_shell_cmd(cd_args));

    char *bruh_args[] = {"bruh", "what"};
    printf("bruh what: %d\n", is_shell_cmd(bruh_args));

    char cwd[4096];
    getcwd(cwd, 4096);
    printf("\n%s$ ", cwd);

    printf("\nRunning cd ..\n");
    if (is_shell_cmd(cd_args)) do_shell_cmd(cd_args);

    getcwd(cwd, 4096);
    printf("%s$ ", cwd);

    printf("\n\nRunning exit\n");
    if (is_shell_cmd(exit_args)) do_shell_cmd(exit_args);
}
