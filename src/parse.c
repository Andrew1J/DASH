#include <stdio.h>
#include "parse.h"

// TODO: finish
// Reads in a line from stdin and does
// related processing to clean it up
// 
// Returns a pointer to the line
char * read_line() {
    char *input = malloc(100 * sizeof(char));

    fgets(input, sizeof(input), stdin);

    return input;
}
