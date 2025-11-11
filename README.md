# 🕹️ Minishell

**Minishell** is a custom Unix shell built in C as part of the 42 curriculum. It supports executing commands with arguments, pipelines, input/output redirection, environment variable handling, and built-in commands such as `cd`, `echo`, `pwd`, `export`, `unset`, and `exit`. The shell manages multiple commands connected with pipes, handles child processes using `fork()` and `execve()`, implements proper signal handling, and ensures robust error management and memory safety, providing a functional minimal shell environment.

📦 Features

- 🧱 Parsing and executing multiple commands with arguments
- ✅ Input and output redirection, including here-documents
- 💡 Built-in commands: `cd`, `echo`, `pwd`, `export`, `unset`, and `exit`
- 🎨 Pipelines connecting commands with proper inter-process communication
- 🖱️ Signal handling for `Ctrl+C`, `Ctrl+D`, and other interrupts
- 💥 Memory-safe and robust against invalid input or command errors

🗺️ Program Rules

- Only valid commands, arguments, and environment variables are accepted
- Supports multiple commands connected via pipes
- Input/output redirections must follow shell syntax
- Actions and updates are handled in real time through the terminal
- The shell exits cleanly with the `exit` command

🚀 Build & Run

⚙️ Requirements

- `gcc` or `clang`
- Unix-based system

🏗️ Compilation

1. Navigate to the project directory:

```bash
cd minishell

    Compile using the provided Makefile:

make

    Generates the executable minishell

    Available make commands:

make clean   # Remove object files
make fclean  # Remove object files and executable
make re      # Recompile everything from scratch

💻 Running the Program

./minishell

    The shell will start and prompt the user to enter commands.

    Example:

minishell$ ls -l | grep txt > output.txt

    This executes the pipeline with proper redirection and stores the output in output.txt.

📁 Notes

    The shell handles multiple commands, pipelines, and redirections in a way similar to a real Unix shell.

    Environment variables and built-in commands are fully supported.

    Memory management and error handling are carefully implemented to prevent leaks and crashes.

This project was developed in collaboration with Lin-0096 Lin Liu.

