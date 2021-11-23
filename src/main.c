#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
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
        	// duplicate stdin, stdout for later
        	int backup_stdin = dup(STDIN_FILENO);
        	if (backup_stdin < 0) {
        		printf("dash: couldn't duplicate stdin, %s\n", strerror(errno));
        		exit(errno);
        	}
        	int backup_stdout = dup(STDOUT_FILENO);
        	if (backup_stdout < 0) {
        		printf("dash: couldn't duplicate stdout, %s\n", strerror(errno));
        		exit(errno);
        	}

        	// parse by redir here
        	// do the redirs here
        	
            char **args = parse_args(commands[i], ' ');

            if (is_shell_cmd(args)) {
                do_shell_cmd(args);
            } else {
                run_command(args);
            }

			// done executing, put stdin stdout back
            dup2(backup_stdout, STDOUT_FILENO);
            dup2(backup_stdin, STDIN_FILENO);
			
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
