#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "parse.h"
#include "run.h"
#include "term_colors.h"

static void sig_handler(int signo);

int main(int argc, char *argv[]) {
	signal(SIGINT, sig_handler);

	int prompt_style = 0;
	if (argc > 1) {
		if (strcmp(argv[1], "--prompt-style") == 0) {
			prompt_style = atoi(argv[2]);
		}
	}

	while (!feof(stdin)) {
		// Print prompt w/temp values for style and like bash
		print_prompt(prompt_style, 0, 0);

		// Read in line from STDIN
		char *input = read_line();

		// Parse the commands from input
		char **commands = parse_tokens(input, ';');

		// Execute commands
		int i = 0;
		while (commands[i]) {
			if (strlen(commands[i]) == 0) {  // don't do anything if there's nothing to do
				i++;
				continue;
			}

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

			char **args = parse_tokens(commands[i], '|');

			// Split line into two strings by the rightmost pipe
			int j = 0;
			char str[4096] = "";
			while (args[j + 1]) {
				strcat(str, args[j]);
				
				if (args[j + 2]) strcat(str," | ");

				j++;
			}

			// Handle line with pipes (if theres a pipe j>=1)
			if (j >= 1) {
				int pipes = do_pipes(str, args[j]);
				if (pipes) {  // failed to redirect at one point or another
					break;  // just stop executing and reprompt
				}
			}

			// Handle line without pipes (if there isn't a pipe j=0)
			else {
				// parse the redirs
				args = parse_tokens(args[0], ' ');

				int redirs = do_redirs(args);
				if (redirs) {  // failed to redirect at one point or another
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
			}

			free(args);
			i++;
		}

		free(input);
		free(commands);
	}

	printf("\n");

	return 0;
}

static void sig_handler(int signo) {
	if (signo == SIGINT) {  // catch SIGINT (Ctrl-C) and do nothing
		return;
	}
}
