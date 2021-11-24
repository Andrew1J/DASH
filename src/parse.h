#ifndef PARSE_H
#define PARSE_H

char * read_line();

char ** parse_tokens(char * line, char ch);

int num_args(char * line, char ch);

char * remove_spaces(char * line);

int do_redirs(char **args);
int reset_redirs(int stdin, int stdout);

int do_pipes(char **args);

#endif
