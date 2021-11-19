# TODO
 * Move line reading to a separate function
    * Dynamically resize input buffer
 * Comments and documentation

# Proposals
 * command.c: Consider changing make_command to accept the command as an argument
 * command.c: Consider adding add_argument to command.c
    * Possibly cleaner than reallocing and adding arguments in parse.c
 * run.c: make helper functions that check what kind of command it is (shell, actual)
    * is_shell_cmd(cmd)  
