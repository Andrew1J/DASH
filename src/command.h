#ifndef COMMAND_H
#define COMMAND_H

struct command {
    int num_args;
    char **args;
};

struct command * make_command();
struct command * free_command(struct command *command);

void print_command(struct command *command);
void command_tests();

#endif
