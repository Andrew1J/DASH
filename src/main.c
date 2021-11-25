#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "parse.h"
#include "run.h"
#include "term_colors.h"

void tests();


int main(int argc, char *argv[]) {
	int prompt_style = 0;
	if (argc > 1) {
		if (strcmp(argv[1], "--prompt-style") == 0) {
			prompt_style = atoi(argv[2]);
		}
	}

	// tests();

	while (1) {
		// Print prompt
		print_prompt(prompt_style);

		// Read in line from STDIN
		char *input = read_line();

		// Parse the commands from input
		// TODO: will have to trim whitespace later
		char **commands = parse_tokens(input, ';');

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

			// parse the redirs
			char **args = parse_tokens(commands[i],' ');

			int redirs = do_redirs(args);
			if (redirs) {  // failed to redirect at one point or another
				break;  // just stop executing and reprompt
			}

			int pipes = do_pipes(args);
			if (pipes) {  // failed to redirect at one point or another
				break;  // just stop executing and reprompt
			}

			if (is_shell_cmd(args)) {
				do_shell_cmd(args);
			} else {
				run_command(args);
			}

			// done executing, put stdin & stdout back
			int rst_redir = reset_redirs(backup_stdin, backup_stdout);
			if (rst_redir) {
				exit(rst_redir);
			}

			free(args);
			i++;
		}

		free(input);
		free(commands);
	}

	return 0;
}
