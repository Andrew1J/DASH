#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

/**
 * Reads in line from STDIN and removes trailing newline
 *
 * @param STDIN
 * @return Returns a pointer to the line
 * @note Still need to implement multiple commands + redirection and piping
 */
char * read_line() {
    char *input = malloc(100 * sizeof(char));

    fgets(input, 100, stdin);

    input[strlen(input) - 1] = '\0';
    return input;
}

/**
 * Takes in STDIN line and puts commands into an array of strings
 *
 * @param Pointer to a string
 * @return Returns an array of strings
 */
char ** parse_args(char * line, char ch) {
    char *curr = line;
    char *token;
    int cnt = num_args(line, ch);
    char ** args = malloc((cnt+1)*sizeof(char *));

    int i=0;
    while((token = strsep(&curr,&ch))) {
        args[i] = token;
        i++;
    }
    args[cnt] = NULL;
    return args;
}

/**
 * Takes in STDIN line and counts the number of arguments
 *
 * @param Pointer to a string
 * @return Returns count of arguments as an integer
 */
int num_args(char * line, char ch) {
    int i=0, num_args=1;
    while(line[i]) {
        if (line[i]==ch) num_args++;
        i++;
    }
    return num_args;
}
