#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "run.h"

// Takes in an array of strings
// and determines if the command
// is a shell command
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

// Takes in a shell command (exit, cd)
// and executes them.
//
// Returns 0 on success, other values
// on failure.
int do_shell_cmd(char **args) {
    char *name = args[0];

    if (strcmp(name, "exit") == 0) {
        exit(0);
    }
    else if (strcmp(name, "cd") == 0) {
        int result = chdir(args[1]);
        if (result) {
            printf("DASH: %s\n", strerror(errno));
            return result;
        }
        return 0;
    }
    
    printf("DASH: Command is not a shell command\n");
    return 1;
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
