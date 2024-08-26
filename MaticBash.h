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
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

class HistoryNode
{
	public:
	int cnum;
	string timestamp;
	string cmd;

	HistoryNode() {}
	HistoryNode(int num, string command)
	{
		cnum = num;
        timestamp = getCurrentTime();
        cmd = command;
    }

    HistoryNode(int num, string tstamp, string command)
	{
		cnum = num;
        timestamp = tstamp;
        cmd = command;
    }

    string getCurrentTime() 
    {
        // Get the current time
        time_t now = time(nullptr);
        struct tm* localTime = localtime(&now);

        // Create a string stream to hold the formatted time
        stringstream ss;
        ss << put_time(localTime, "%Y-%m-%d %H:%M:%S");

        return ss.str();
    }

};
class History
{
	vector<HistoryNode> history;
	int nextCommandNumber;
	string historyFilePath;

	public:
	History() : nextCommandNumber(1) {
		historyFilePath = "./.maticbash_history";
		loadHistory();
	}

	void addCommand(const string& cmd) {
		
        history.emplace_back(HistoryNode(nextCommandNumber, cmd));
        nextCommandNumber++;
    }

	string getCommand(int index) const {
        if (index >= 0 && index < history.size()) {
            return history[index].cmd;
        }
        return "";
    }

    int size() const {
        return history.size();
    }

    void clear() {
        history.clear();
        nextCommandNumber = 1;
    }

    void eraseFirst() {
        history.erase(history.begin());
    }

    void saveHistory() const 
    {
        ofstream file(historyFilePath);
        if (file.is_open()) 
        {
            for (const auto& node : history) 
            {
                file << node.cnum << " " << node.timestamp << '\t' << node.cmd << std::endl;
            }
            file.close();
        } 
        else 
        {
            cerr << "Unable to open history file for writing." << std::endl;
        }
    }

    void loadHistory() {
        ifstream file(historyFilePath);
        if (file.is_open()) 
        {
            history.clear();
            string line;

            
            while (getline(file, line)) 
            {
            	stringstream ss(line);
            	int cnum;
	            string timestamp;
	            string cmd;
                ss >> cnum;
                cout << cnum << endl;
                ss.ignore(); // Ignore the space after cnum
                getline(ss, timestamp, '\t');
                cout << timestamp << endl;
                getline(ss, cmd);
                cout << cmd << endl;

                history.emplace_back(HistoryNode(cnum, timestamp, cmd));
                nextCommandNumber = max(nextCommandNumber, cnum + 1);
            }
            file.close();
        }
    }

    void display() const {
        for (const auto& node : history) {
            cout << node.cnum << "  " << node.timestamp << " " << node.cmd << endl;
        }
    }
};


#define MAX_INPUT_SIZE 1000
#define HISTORY_SIZE 100



// Function declarations
void add_to_history(const string& command);
string read_input();
vector<string> parse_input(const string& input);
void execute_command(const vector<string>& args);
bool execute_builtin(const vector<string>& args);
void signal_handler(int signo);
string expand_env_var(const string& arg);
void refreshLine(int cursor_pos);

#endif // MATICBASH_H
