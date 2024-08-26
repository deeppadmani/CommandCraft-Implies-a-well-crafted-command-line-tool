# CommandCraft: Implies a well crafted command line tool


## Overview

This project is a custom shell implementation in C++. The shell provides basic command-line functionalities, such as executing commands, handling built-in commands like `cd`, managing a command history, and supporting arrow key navigation (up, down, left, right) along with backspace functionality. The shell is designed to mimic the behavior of a typical Unix shell with minimal features.

## Features

- **Command Execution**: Execute external programs using `execvp`.
- **Built-in Commands**: Supports basic built-in commands such as `cd`.
- **Command History**: Navigate through previous commands using the up and down arrow keys.
- **Arrow Key Navigation**: Navigate through the command line using left and right arrow keys.
- **Backspace Support**: Delete characters from the command line using the backspace key.
- **Signal Handling**: Handles signals like `SIGINT` (Ctrl+C) and `SIGTERM`.
- **Persistent History**: Command history is stored in `.maticbash_history` file and persists between sessions.

## File Structure

- **MaticMain.cpp**: Main entry point for the shell, handles input/output and calls other components.
- **MaticBash.cpp**: Contains the core logic for command parsing, execution, and history management.
- **.maticbash_history**: Stores command history across sessions.
## Compilation

The project uses a `Makefile` to manage the build process. The following are the key targets:

- **`Matic`**: The main executable target. It compiles all source files into the final executable.
- **`clean`**: Removes all object files and the executable.

## Prerequisites

- **Compiler**: A C++ compiler supporting C++11 (e.g., g++).
- **Operating System**: Linux or any Unix-based system.

## How to Build

1. Open the terminal and navigate to the project directory.
2. Run the following command to build the project:

   ```bash
   make
   ```

3. The executable named `Matic` will be created in the project directory.

## How to Clean

To remove the compiled executable and object files, run:

```bash
make clean
```

## Usage

Once compiled, you can run the shell by executing the following command:

```bash
./Matic
```

You'll see a prompt (`$ `) where you can enter commands. Use arrow keys to navigate through command history or move the cursor within the command line. Press Enter to execute a command.

### History Feature

The shell now includes a persistent history feature:

- Command history is stored in a file named `.maticbash_history` in the user's home directory.
- This allows the history to persist between sessions.
- Users can access previous commands even after restarting the shell.
- The up and down arrow keys can be used to navigate through the command history.

## Author

**Deep Padmani**  
Date: August 25, 2024

## License

This project is open source and available under the MIT License.
