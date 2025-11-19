# Minishell

**Minishell** is a custom Unix shell built in C as part of the 42 curriculum. It supports executing commands with arguments, pipelines, input/output redirection, environment variable handling, and built-in commands such as `cd`, `echo`, `pwd`, `export`, `unset`, and `exit`. The shell manages multiple commands connected with pipes, handles child processes using `fork()` and `execve()`, implements proper signal handling, and ensures robust error management and memory safety, providing a functional minimal shell environment.


## Features
- Parsing and executing multiple commands with arguments
- Input and output redirection, including here-documents
- Built-in commands: `cd`, `echo`, `pwd`, `export`, `unset`, and `exit`
- Pipelines connecting commands with proper inter-process communication
- Signal handling for `Ctrl+C`, `Ctrl+D`, and other interrupts
- Memory-safe and robust against invalid input or command errors


## Program Rules
- Only valid commands, arguments, and environment variables are accepted
- Supports multiple commands connected via pipes
- Input/output redirections must follow shell syntax
- Actions and updates are handled in real time through the terminal
- The shell exits cleanly with the `exit` command


## Build & Run

1. Clone the Module Directory
```
git clone https://github.com/Yuxin29/minishell.git minishell
cd minishell
```

2. Compile and Run Your Program
```
make
```
Other available commands:
```
make clean   # Remove object files
make fclean  # Remove object files and executable
make re      # Recompile everything from scratch
```

3. Running the Program
4. The shell will start and prompt the user to enter commands.
```
./minishell
```
Example:
```
minishell$ ls -l | grep txt > output.txt
# This executes the pipeline with proper redirection and stores the output in output.txt.
```


## Notes
- The shell handles multiple commands, pipelines, and redirections in a way similar to a real Unix shell.
- Environment variables and built-in commands are fully supported.
- Memory management and error handling are carefully implemented to prevent leaks and crashes.


## co-developer
This project was developed in collaboration with Lin-0096 Lin Liu.

