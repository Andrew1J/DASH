#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"
#include "run.h"
#include "command.h"

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void tests();

int main(int argc, char *argv[]) {
	// tests();

	char cwd[4096];  // see https://www.google.com/search?q=unix+path+length

	while (1) {
        // Print prompt
		getcwd(cwd, 4096);
		printf("%s" RED " >" RESET GRN ">" RESET BLU "> " RESET, cwd);

        // Read in line from STDIN
        char *input = read_line();

        // Parse the commands from input
        // TODO: will have to trim whitespace later
        char **commands = parse_args(input, ';');

        // Execute commands
        int i = 0;
        while (commands[i]) {
            char **args = parse_args(commands[i], ' ');

            if (is_shell_cmd(args)) {
                do_shell_cmd(args);
            } else {
                run_command(args);
            }
			
			free(args);
            i++;
        }

		free(input);
		free(commands);
	}

    return 0;
}


void tests() {
	command_tests();
	run_tests();

    exit(0);
}
