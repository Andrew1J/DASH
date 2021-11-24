#ifndef RUN_H
#define RUN_H

void dash_loop();

void print_prompt();

int is_shell_cmd(char **args);
int do_shell_cmd(char **args);

int run_command(char **args);

int do_redirs(char **args);
int reset_redirs(int stdin, int stdout);

int do_pipes(char **args);

void run_tests();

#endif
