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
