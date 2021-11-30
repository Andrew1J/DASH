# ![DASH](https://i.imgur.com/j3yIxXB.gif)
(**D**ylan and **A**ndrew's **SH**ell) <br>
This project is an implementation of a shell in C for Systems level programming.

<sup><sub>Disclaimer: this project's name and it's potential similarities to other shells of similar naming is entirely coincidental. Dylan was an idiot and forgot to check if the name was already used by another shell and now we're too lazy to change it. Idiot.</sub></sup>

## Getting Started

* Clone the Repository
```
git clone https://github.com/Andrew1J/DASH.git
```
* Change Directories
```
cd DASH
```
* Compile using make
```
make
```
* Run DASH using make, or by running the binary directly (with any arguments you want)

`make run` or `./bin/dash` or `./bin/dash --prompt-style 3`

* Exit DASH
``` 
exit
```

## Description
### Features Our Shell Implements
- Execute commands with arguments (duh)
- Multiple commands in one line using `;`
- Redirection of stdin and stdout
  - Behavior mimics that of bash
  - Implements `<`, `>`, `>>`
- Piping and multipiping

### Good things to know (quirks and new features)
- Change prompt styling using a command-line argument on invocation of dash
  - Invoke dash with `--prompt-style <style>` where `<style>` is an integer from 0-3, inclusive
  - Defaults to style 0
  - Invalid inputs default to style 0
  - Example: `dash --prompt-style 3`
  - Styles
    - Style 0: Weird style with username, hostname, and cwd on a separate line
    - Style 1: Cool style with colorful triangles before user input
    - Style 2: Colorful username, hostname, and cwd, and a lightning bolt before user input
    - Style 3: Mimics bash, very realistic
- Inputs like `;;;;;;;` evaluate to nothing, and execute nothing
  - In bash, this would give syntax errors

### Limitations
- The maximum input length per line is 498 characters
- The maximum length of the current directory is 4096 characters, but it should be the limit for UNIX complient systems anyway
- The maximum length of the username is 4096 characters, but the system's limit is likely lower
- The maximum length of the hostname is 4096 characters, but the system's limit is likely lower

### Bugs
- Tabbing and then hitting backspace on an empty input may behave weirdly, but we've only observed it to be a visual glitch and it likely won't affect anything else

### Function Headers
***parse.c***: (parses input to be run by run.c)
```C
char * read_line();
 - Reads in line from STDIN and removes trailing newline
 - @param STDIN
 - @return Returns a pointer to the line

char ** parse_tokens(char * line, char ch);
 - Takes in STDIN line and puts commands into an array of strings
 - @param line pointer to a string
 - @param ch delimiter to split the string by
 - @return An array of strings

int num_args(char * line, char ch);
 - Takes in a line and counts the number of arguments
 - @param line pointer to a string
 - @param ch delimiter to split the string by
 - @return Count of arguments as an integer

char * remove_spaces(char * line);
 - Takes in a line and trims whitespace and tabs before/after line
 - @param line pointer to a string
 - @return Returns a pointer to the trimmed line

```

***run.c***: (executes commands passed by parse.c)
```C
void print_prompt();
 - Prints the shell prompt. Choose from 4 styles.
 - @param prompt_style selects prompt style, overridden by @param like_bash
 - @param prompt_path_colors enables path coloring, overridden by @param like_bash (not enabled yet)
 - @param like_bash makes dash look like bash! (not enabled yet)

int is_shell_cmd(char **args);
 - Determines if the given argument is a shell command
 - @param args Array of arguments
 - @return 1 if true, 0 if false

int do_shell_cmd(char **args);
 - Takes in a shell command and executes them
 - @param args Array of arguments
 - @return 0 on success and other values on failure

int run_command(char **args);
 - Takes in a regular command and executes it
 - @param args Array of arguments
 - @return 0 on success and other values on failure

int do_redirs(char **args);
 - Takes in an array of arguments and sets up redirects
 - @param args pointer to an array of strings
 - @return 0 if successful, other values on failure
 - @note dup2 should close previously opened files, but it fails silently, so :/

int reset_redirs(int stdin, int stdout);
 - Resets stdin, stdout to backed up values
 - @param stdin file descriptor to replace stdin
 - @param stdout file descriptor to replace stdout
 - @return 0 on success, other values on failure

int do_pipes(char *args1, char *args2);
 - Takes in an array of arguments and sets up pipes
 - @param args pointer to an array of strings
 - @return 0 if successful, other values on failure

```
***main.c***: (puts everything together and runs DASH)
```C
int main();
 - Runs the shell
 
static void sig_handler(int signo);
 - Handles SIGINT signals (does nothing basically)
 - @param signo signal received
```
### Possible Additional Features TODO
 * Signal Handling
 * Declutter main.c
 * String Handling (echo "hello" => hello)
 * Cooler prompt options! (time date, host, smiley faces, colors)
 * Improved organization
 * Implement more commands unique to our shell
    - Prompt selection command (rather than selecting before running)
    - Something like "cowsay"
