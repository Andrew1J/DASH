#ifndef PARSE_H
#define PARSE_H

char * read_line();

char ** parse_tokens(char * line, char ch);

int num_args(char * line, char ch);

char * remove_spaces(char * line);

#endif
