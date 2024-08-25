
/**
 * @file MaticBash.cpp
 * @brief Implementation of the custom shell functions.
 *
 * This file contains the implementation of core shell functionalities
 * including input reading, command parsing, and command execution.
 *
 * @author Deep Padmani
 * @date August 25, 2024
 */
#include "MaticBash.h"

// Global history object
vector<string> history;
int history_index = -1;

/**
 * @brief Adds a new command to the history.
 *
 * @param command The command string to add to history.
 */
void add_to_history(const string& command) {
    if (history.size() < HISTORY_SIZE) {
        history.push_back(command);
    } else {
        history.erase(history.begin());
        history.push_back(command);
    }
    history_index = history.size(); // Reset history index after new command
}

/**
 * @brief Retrieves the command in the history according to index.
 *
 * @return string The previous command, or an empty string if at the beginning.
 */
string get_history(int index) {
    if (index >= 0 && index < history.size()) {
        return history[index];
    }
    return "";
}

/**
 * @brief Reads input from the user with advanced terminal handling.
 *
 * This function implements a custom input reading mechanism with
 * support for arrow key navigation and command history.
 *
 * @return string The user's input command.
 */
string read_input() 
{
    struct termios oldt, newt;
    string input;
    int cursor_pos = 0;

    // Save current terminal settings
    if (tcgetattr(STDIN_FILENO, &oldt) != 0) 
    {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Modify terminal settings for character-by-character input
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    while (true) {
        char ch;
        if (read(STDIN_FILENO, &ch, 1) != 1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        if (ch == '\n') 
        {   // End of input
            cout << endl;
            break;
        }  
        else if (ch == 127) 
        {   // Backspace
            if (cursor_pos > 0) {
                input.erase(cursor_pos - 1, 1);
                cursor_pos--;
                cout << "\b \b";
                cout.flush();
                cout << input.substr(cursor_pos) << " ";
                cout.flush();
                cout << "\b";
                cout.flush();
                for (int i = cursor_pos; i < input.size(); i++) cout << "\b";
            }
        } 
        else if (ch == 27) 
        {   // Arrow keys
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) != 1 || read(STDIN_FILENO, &seq[1], 1) != 1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            if (seq[0] == '[') 
            {
                if (seq[1] == 'A') 
                {   // Up arrow - previous command
                    if (history_index > 0) 
                    {
                        history_index--;
                        for (int i = cursor_pos; i; i--)
                        {
                            cout << "\b \b";
                            cout.flush();
                        }
                        input = get_history(history_index);
                        cursor_pos = input.length();
                        cout << "\r$ " << input;
                        cout.flush();
                    }
                } 
                else if (seq[1] == 'B') 
                {   // Down arrow - next command
                    if (history_index < history.size() - 1) 
                    {
                        history_index++;
                        for (int i = cursor_pos; i; i--)
                        {
                            cout << "\b \b";
                            cout.flush();
                        }
                        input = get_history(history_index);
                        cursor_pos = input.length();
                        cout << "\r$ " << input;
                        cout.flush();
                    } 
                    else 
                    {
                        history_index = history.size();
                        for (int i = cursor_pos; i; i--)
                        {
                            cout << "\b \b";
                            cout.flush();
                        }
                        input.clear();
                        cursor_pos = 0;
                        cout << "\r$ ";
                        cout.flush();
                    }
                } 
                else if (seq[1] == 'C') 
                {   // Right arrow
                    if (cursor_pos < input.size()) 
                    {
                        cout << input[cursor_pos];
                        cout.flush();
                        cursor_pos++;
                    }
                } 
                else if (seq[1] == 'D') 
                {   // Left arrow
                    if (cursor_pos > 0) {
                        cout << "\b";
                        cout.flush();
                        cursor_pos--;
                    }
                }
            }
        } 
        else 
        {
            // Regular character input
            if (cursor_pos < input.size()) 
            {
                input.insert(cursor_pos, 1, ch);
            } 
            else 
            {
                input += ch;
            }
            cursor_pos++;
            cout << "\r$ " << input;
            cout.flush();

            for (int i = cursor_pos; i < input.size(); i++) 
            {
                cout << "\b";
                cout.flush();
            }
        }
    }

    // Restore original terminal settings
    if (tcsetattr(STDIN_FILENO, TCSANOW, &oldt) != 0) 
    {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    return input;
}

/**
 * @brief Parses the input string into a vector of arguments.
 *
 * This function splits the input string into separate arguments,
 * handling quoted strings appropriately.
 *
 * @param input The input string to parse.
 * @return vector<string> A vector of parsed arguments.
 */
vector<string> parse_input(const string& input) {

    vector<string> args;
    string current_arg;
    bool in_quote = false;
    char quote_char = '\0';

    for (char c : input) {
        if (!in_quote && (c == '"' || c == '\'')) {
            in_quote = true;
            quote_char = c;
        } else if (in_quote && c == quote_char) {
            in_quote = false;
            quote_char = '\0';
        } else if (!in_quote && isspace(c)) {
            if (!current_arg.empty()) {
                args.push_back(expand_env_var(current_arg));
                current_arg.clear();
            }
        } else {
            current_arg += c;
        }
    }

    if (!current_arg.empty()) {
        args.push_back(expand_env_var(current_arg));
    }

    if (args.size() >= MAX_INPUT_SIZE) {
        cerr << "Error: Too many arguments\n";
    }

    return args;
}

/**
 * @brief Executes built-in shell commands.
 *
 * This function handles the execution of built-in commands like 'cd'.
 *
 * @param args The vector of command arguments.
 * @return bool Returns true if the command is not a built-in, false otherwise.
 */
bool execute_builtin(const vector<string>& args) {
    if (args[0] == "cd") {
        if (args.size() < 2) {
            cerr << "cd: missing argument" << endl;
        } else {
            if (chdir(args[1].c_str()) != 0) {
                perror("cd");
            }
        }
        return false;
    }
    return true;
}
/**
 * @brief Executes external commands.
 *
 * This function forks a new process to execute external commands.
 *
 * @param args The vector of command arguments.
 */
void execute_command(const vector<string>& args) {
    pid_t pid = fork();
    if (pid == -1) 
    {
        perror("fork");
    } 
    else if (pid == 0) 
    {
        // Child process
        vector<char*> argv;
        for (const string& arg : args) 
        {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }
        argv.push_back(nullptr);

        execvp(argv[0], argv.data());
        perror("execvp");
        exit(1);
    } 
    else 
    {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) 
        {
            cerr << "error: command exited with code " << WEXITSTATUS(status) << endl;
        }
    }
}

/**
 * @brief Handles SIGINT and SIGTERM signals.
 *
 * This function provides a custom response to interrupt signals.
 *
 * @param signo The signal number received.
 */
void signal_handler(int signo) 
{
    switch(signo)
    {
        case SIGINT:
        case SIGTERM:
        case SIGTSTP:
        default:
            cout << "\n$ ";
            cout.flush();
            break;
    }
}
/**
 * @brief Expands environment variables in a given string.
 *
 * This function scans through the input string and looks for environment variables 
 * indicated by the `$` character followed by alphanumeric characters or underscores (e.g., `$HOME`). 
 * It replaces these variables with their corresponding values from the environment, 
 * as retrieved by the `getenv` function.

 *
 * @param arg The input string potentially containing environment variables.
 * @return A new string with the environment variables expanded to their corresponding values.
 */
string expand_env_var(const string& arg) 
{
    string expanded_arg;
    size_t pos = 0;
    while (pos < arg.length()) {
        if (arg[pos] == '$') {
            size_t end = pos + 1;
            while (end < arg.length() && (isalnum(arg[end]) || arg[end] == '_')) {
                ++end;
            }
            string var_name = arg.substr(pos + 1, end - pos - 1);
            const char* var_value = getenv(var_name.c_str());
            if (var_value) {
                expanded_arg += var_value;
            }
            pos = end;
        } else {
            expanded_arg += arg[pos];
            ++pos;
        }
    }
    return expanded_arg;
}