#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
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
char ** parse_args(char * line, char ch) {
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

/**
 * Takes in a line and splits it by spaces
 *
 * @param line pointer to a string
 * @return a pointer to an array of strings
 */
char ** parse_redirs(char *line) {
    
}

/**
 * Takes in an array of arguments and sets up redirects
 *
 * @param args pointer to an array of strings
 * @return 0 if successful, other values on failure
 * @note dup2 should close previously opened files, but it fails silently, so :/
 */
int do_redirs(char **args) {
	int i;
    for (i = 0; args[i]; i++) {
        if (!strcmp(args[i], "<")) {  // open file for reading
            int file = open(args[i + 1], O_RDONLY);
            if (file < 0) {
                printf("dash: failed to redirect stdin, %s\n", strerror(errno));
                return errno;
            }

            int dup2_result = dup2(file, STDIN_FILENO);
            if (dup2_result < 0) {
                printf("dash: couldn't replace stdin, %s\n", strerror(errno));
            }

            args[i] = NULL;
        }
        else if (!strcmp(args[i], ">")) {
            int file = open(args[i + 1], O_WRONLY | O_CREAT, 0644);
            if (file < 0) {
                printf("dash: failed to redirect stdout, %s\n", strerror(errno));
                return errno;
            }

            int dup2_result = dup2(file, STDOUT_FILENO);
            if (dup2_result < 0) {
                printf("dash: couldn't replace stdout, %s\n", strerror(errno));
            }

            args[i] = NULL;
        }
        else if (!strcmp(args[i], ">>")) {
            int file = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (file < 0) {
                printf("dash: failed to redirect stdout, %s\n", strerror(errno));
                return errno;
            }

            int dup2_result = dup2(file, STDOUT_FILENO);
            if (dup2_result < 0) {
                printf("dash: couldn't replace stdout, %s\n", strerror(errno));
            }

            args[i] = NULL;
        }
    }

	return 0;
}

/**
 * Resets stdin, stdout to backed up values
 *
 * @param stdin file descriptor to replace stdin
 * @param stdout file descriptor to replace stdout
 * @returns 0 on success, other values on failure
 */
int reset_redirs(int stdin, int stdout) {
    int dup2_result_out = dup2(stdout, STDOUT_FILENO);
    if (dup2_result_out < 0) {
        printf("dash: couldn't replace stdin, %s\n", strerror(errno));
        return errno;
    }
    int dup2_result_in = dup2(stdin, STDIN_FILENO);
    if (dup2_result_out < 0) {
        printf("dash: couldn't replace stdout, %s\n", strerror(errno));
        return errno;
    }

    return 0;
}
