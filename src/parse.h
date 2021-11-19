#ifndef PARSE_H
#define PARSE_H

char * read_line();

int num_args(char * line);

char ** parse_args( char * line );

#endif
