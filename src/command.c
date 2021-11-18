#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"


struct command * make_command() {
    return calloc(sizeof(struct command), 1);
}

struct command * free_command(struct command *cmd) {
    int i;
    for (i = 0; i < cmd->num_args; i++) {
        free(cmd->args[i]);
    }
    free(cmd);
}

void print_command(struct command *cmd) {
    int i;
    for (i = 0; i < cmd->num_args; i++) {
        printf("%s ", cmd->args[i]);
    }
    printf("\n");
}

void command_tests() {
    struct command *cmd = make_command();
	cmd->num_args = 2;
	cmd->args = calloc(cmd->num_args, sizeof(char *));

	char *args[2] = {"bruh", "what"};

	int i;
	for (i = 0; i < cmd->num_args; i++) {
		cmd->args[i] = malloc(100);
		strcpy(cmd->args[i], args[i]);
	}

	print_command(cmd);
	free_command(cmd);
}
