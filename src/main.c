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
		print_prompt();

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

			// // parse the redirs
			// char **parsed_redirs = parse_redirs(commands[i]);
			// int redirs = do_redirs(parse_redirs);
			// if (redirs) {  // failed to redirect at one point or another
			// 	break;  // just stop executing and reprompt
			// }

			// char **args = parse_args(parsed_redirs[0], ' ');

			char **args = parse_args(commands[i], ' ');

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


void tests() {
	char *redir_args[] = {"ls -l -a", ">", "ls_out", 0};
	do_redirs(redir_args);
	char *args[] = {"ls", "-l", "-a", 0};

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

	run_command(args);

	int rst_redir = reset_redirs(backup_stdin, backup_stdout);
	if (rst_redir) {
		exit(rst_redir);
	}

	exit(0);

	command_tests();
	run_tests();

	exit(0);
}
