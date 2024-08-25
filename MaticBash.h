/**
 * @file MaticBash.h
 * @brief Header file for the custom shell implementation.
 *
 * This file contains declarations for the main shell functions and
 * necessary includes for the shell implementation.
 *
 * @author Deep Padmani
 * @date August 25, 2024
 */
#ifndef MATICBASH_H
#define MATICBASH_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <sys/wait.h>

using namespace std;

#define MAX_INPUT_SIZE 1000
#define HISTORY_SIZE 100

// Function declarations
void add_to_history(const string& command);
string get_history(int index);
string read_input();
vector<string> parse_input(const string& input);
void execute_command(const vector<string>& args);
bool execute_builtin(const vector<string>& args);
void signal_handler(int signo);
string expand_env_var(const string& arg);

#endif // MATICBASH_H