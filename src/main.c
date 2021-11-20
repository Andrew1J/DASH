#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"
#include "run.h"
#include "command.h"

void tests();

int main () {
	// tests();

	char input[100]; // TODO: make it expand to the input size
	char cwd[4096];  // see https://www.google.com/search?q=unix+path+length

	while (1) {
        // Print prompt
		getcwd(cwd, 4096);
		printf("%s $ ", cwd);

        // Read in line from STDIN
        char *input = read_line();

        // Parse the args from input
        char ** args = parse_args( input );

        // Execute commands
        execvp(args[0], args);

        // Exit shell
		if (strcmp(input, "exit") == 0) break;
	}

    return 0;
}


void tests() {
	command_tests();
	run_tests();
}
