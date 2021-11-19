#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

// TODO: finish
// Reads in a line from stdin and does
// related processing to clean it up
//
// Returns a pointer to the line
char * read_line() {
    char *input = malloc(100 * sizeof(char));

    fgets(input, 100, stdin);

    input[strlen(input) - 1] = '\0';
    return input;
}

int num_args(char * line) {
    char *curr = line;
    char *token;
    int num_args=0;
    while((token = strsep(&curr," "))) {
        num_args++;
    }
    return num_args;
}

char ** parse_args( char * line ) {
    printf("%s\n", i);
    char *curr = line;
    char *token;
    int cnt = num_args(line);
    char ** args = malloc(cnt*sizeof(char *));

    int i=0;
    while((token = strsep(&curr," "))) {
        args[i] = token;
        i++;
    }
    return args;
}
