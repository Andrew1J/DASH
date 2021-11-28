#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include "run.h"
#include "term_colors.h"
/**
 * Prints the shell prompt. Choose from 4 styles.
 *
 * @param prompt_style selects prompt style, overridden by @param like_bash
 * @param prompt_path_colors enables path coloring, overridden by @param like_bash
 * @param like_bash makes dash look like bash!
 */
void print_prompt(int prompt_style, int prompt_path_colors, int like_bash) {
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));

    char username[4096];
	char *_username = getenv("LOGNAME");
	if (_username) {  // try getting the username from the environment variable LOGNAME
		strcpy(username, _username);
	} else {
    	getlogin_r(username, sizeof(username));
	}
	

    // Getting Hostname
    char hostname[4096];
    gethostname(hostname, sizeof(hostname));

    if (like_bash) {
        prompt_path_colors = 1;
        prompt_style = 3;
    }

	switch(prompt_style) {
		case 0:
		    // Style 1
		    printf("╔" YEL " %s" RESET "⚡" RED"%s\n" RESET, hostname, cwd);
		    printf("╚▶ ");
		    break;

	    case 1:
		    // Style 2
		    printf("⚡");
		    printf("%s" RED " ▶" RESET GRN "▶" RESET BLU "▶ " RESET, cwd);
			break;

		case 2:
		    // Style 3
		   printf(YEL "%s" RESET ":" RED "%s" RESET "⚡ ", hostname, cwd);
			break;
        case 3:
            // Style 4, like bash
            printf(BGRN "%s@%s" RESET ":" BBLU "%s" RESET "$ ", username, hostname, cwd);
            break;
	}
}

/**
 * Determines if the given argument is a shell command
 *
 * @param args Array of arguments
 * @return 1 if true, 0 if false
 */
int is_shell_cmd(char **args) {
    char *name = args[0];

    if (strcmp(name, "exit") == 0) {
        return 1;
    }
    else if (strcmp(name, "cd") == 0) {
        return 1;
    }
    else return 0;
}

/**
 * Takes in a shell command and executes them
 *
 * @param args Array of arguments
 * @return 0 on success and other values on failure
 */
int do_shell_cmd(char **args) {
    char *name = args[0];

    if (strcmp(name, "exit") == 0) {
        exit(0);
    }

    else if (strcmp(name, "cd") == 0) {
        int result = chdir(args[1]);
        if (result) {
            printf("dash: %s\n", strerror(errno));
            return result;
        }
        return 0;
    }

    return 1;
}

/**
 * Takes in a regular command and executes it
 *
 * @param args Array of arguments
 * @return 0 on success and other values on failure
 */
int run_command(char **args) {
    int f = fork();

    if (f < 0) {  // error
        printf("dash: couldn't fork! %s\n", strerror(errno));
        return errno;
    }
    else if (f > 0) {  // parent
        int status;
        int pid = wait(&status);

        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }

        return -1;
    }
    else {  // child
        if (execvp(args[0], args)) {
        	printf("Dash ⚡: %s\n", strerror(errno));
        }
        exit(errno);
    }
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
            int file = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
 * @note change stdin and stdout variable names (reserved keywords)
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

/**
 * Takes in an array of arguments and sets up pipes
 *
 * @param args pointer to an array of strings
 * @return 0 if successful, other values on failure
 */
int do_pipes(char *args1, char *args2) {
    FILE *pipein, *pipeout;
    char buffer[4096];

    if ((pipein = popen(args1, "r")) == NULL) {
        perror("popen");
        exit(1);
    }

    if ((pipeout = popen(args2, "w")) == NULL) {
        perror("popen");
        exit(1);
    }

    // Get output from pipein stream and direct it into pipeout stream
    while(fgets(buffer, 4096, pipein)) {
        fputs(buffer, pipeout);
    }

    // Close the pipes
    pclose(pipein);
    pclose(pipeout);
	return 0;
}

void run_tests() {
    printf("\n\nrun.c Tests:\n");

    char *exit_args[] = {"exit"};
    printf("exit: %d\n", is_shell_cmd(exit_args));

    char *cd_args[] = {"cd", ".."};
    printf("cd ..: %d\n", is_shell_cmd(cd_args));

    char *bruh_args[] = {"bruh", "what"};
    printf("bruh what: %d\n", is_shell_cmd(bruh_args));

    char cwd[4096];
    getcwd(cwd, 4096);
    printf("\n%s$ ", cwd);

    printf("\nRunning cd ..\n");
    if (is_shell_cmd(cd_args)) do_shell_cmd(cd_args);

    getcwd(cwd, 4096);
    printf("%s$ ", cwd);

    printf("\n\nRunning exit\n");
    if (is_shell_cmd(exit_args)) do_shell_cmd(exit_args);
}
