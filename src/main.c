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
		getcwd(cwd, 4096);
		printf("%s $ ", cwd);

        char *input;
        input = read_line();
        printf("%s\n", input);  // just echo it for now
        char ** args = parse_args( input );



        execvp(args[0], args);

		if (strcmp(input, "exit") == 0) break;

	}

    return 0;
}


void tests() {
	command_tests();
	run_tests();
}
