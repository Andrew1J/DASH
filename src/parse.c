#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "parse.h"

/**
 * Reads in line from STDIN and removes trailing newline
 *
 * @param STDIN
 * @return Returns a pointer to the line
 * @note Still need to implement multiple commands + redirection and piping
 */
char * read_line() {
    char *input = calloc(500, sizeof(char));

    if (!input) {
        printf("dash: could not allocate memory for user input\n");
        exit(EXIT_FAILURE);
    }

    fgets(input, 500, stdin);

    input[strlen(input) - 1] = '\0';
    return input;
}

/**
 * Takes in STDIN line and puts commands into an array of strings
 *
 * @param line pointer to a string
 * @param ch delimiter to split the string by
 * @return An array of strings
 */
char ** parse_tokens(char * line, char ch) {
    char *curr = line;
    char *token;
    int cnt = num_args(line, ch);
    char **args = calloc((cnt + 1), sizeof(char *));

    int i = 0;
    while ((token = strsep(&curr, &ch))) {
        args[i] = remove_spaces(token);
        i++;
    }
    args[cnt] = NULL;

    return args;
}

/**
 * Takes in a line and counts the number of arguments
 *
 * @param line pointer to a string
 * @param ch delimiter to split the string by
 * @return Count of arguments as an integer
 */
int num_args(char * line, char ch) {
    int i=0, num_args=1;

    while (line[i]) {
        if (line[i] == ch) num_args++;
        i++;
    }

    return num_args;
}

/**
 * Takes in a line and removes whitespace
 *
 * @param line pointer to a string
 */
char * remove_spaces(char * line) {
    // Remove from beginning
    while (*line==' ' || *line=='\t') {
        line++;
    }

    // Remove from end
    char *end = line + strlen(line) - 1;
    while(end > line && (*end == ' '||*end=='\t')) {
        end--;
    }

    // Add terminating null
    *(end+1) = '\0';
    return line;
}
