/**
 * @file MaticMain.cpp
 * @brief Main entry point for the custom shell program.
 *
 * This file contains the main function that initializes the shell,
 * sets up signal handlers, and runs the main input loop.
 *
 * @author Deep Padmani
 * @date August 25, 2024
 */
#include "MaticBash.h"

// Global history object
extern History commandHistory;
extern int history_index;

int main() 
{
    // Set up signal handlers for SIGINT and SIGTERM
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGTSTP, signal_handler);
    
    history_index = commandHistory.size();

    while (true) 
    {
        // Display prompt
        cout << "$ ";
        cout.flush();

        // Read user input
        string input = read_input();


        // Check if input exceeds maximum allowed length
        if (input.length() >= MAX_INPUT_SIZE - 1) {
            cerr << "Error: Command line too long" << endl;
            continue;
        }

        // Parse input into arguments
        vector<string> args = parse_input(input);

        // Skip empty input
        if (args.empty()) {
            continue;
        }

        // Add the command to history
        add_to_history(input);

        // Check for exit command
        if (args[0] == "exit") {
            commandHistory.saveHistory();
            exit(0);
        }

        // Execute built-in commands
        if (!execute_builtin(args)) {
            continue;
        }

        // Execute external commands
        execute_command(args);
    }
    return 0;
}
