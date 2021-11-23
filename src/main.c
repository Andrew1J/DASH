#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"
#include "run.h"
#include "command.h"
#include "term_colors.h"

void tests();

int main(int argc, char *argv[]) {
	// tests();

	char cwd[4096];  // see https://www.google.com/search?q=unix+path+length

	while (1) {
        // Print prompt
		print_prompt();

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
