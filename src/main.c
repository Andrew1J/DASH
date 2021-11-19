#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parse.h"
#include "run.h"
#include "command.h"

void tests();

int main () {
	tests();

	char input[100]; // TODO: make it expand to the input size
	char cwd[4096];  // see https://www.google.com/search?q=unix+path+length

	while (1) {
		getcwd(cwd, 4096);
		printf("%s $ ", cwd);

        char * input = read_line();
        // fgets(input, sizeof(input), stdin);
		// input[strlen(input) - 1] = '\0';

        printf("%d\n", num_args(input));

        // printf("%s\n", input);
		// fgets(input, sizeof(input), stdin);
		// input[strlen(input) - 1] = '\0';  // remove trailing newline
        //
		if (strcmp(input, "exit") == 0) break;
        //
		// printf("%s\n", input);  // just echo it for now
	}

    return 0;
}


void tests() {
	command_tests();
	run_tests();
}
