
#include "debug.h"
#include "shell.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>

using namespace std;

void Shell::run() {
    string cmdLine;
    while (true) {
        cout << "$ ";
        getline(cin, cmdLine);
        if (cin.eof()) break;
        executeCommand(cmdLine);
    }
}

void Shell::executeCommand(const string& cmdLine) {
    auto tokens = tokenize(cmdLine);
    bool inBackground = isBackgroundCommand(tokens);
    if (inBackground) {
        tokens.pop_back(); // Remove the "&"
    }
    auto pipeSections = splitPipes(tokens);

    PRINT(__LOCATION__)
    PRINT("\n")
    for(auto cmd: pipeSections)
    {
        PRINT(__LOCATION__)
        for(auto arg: cmd)
        {
            PRINT("   " << arg << "   ")
        }
        PRINT("\n")
    }
    PRINT("\n")

    int pipes[2][2], in_fd = 0;

    for (size_t i = 0; i < pipeSections.size(); ++i) {
        auto& section = pipeSections[i];
        bool isLast = i == pipeSections.size() - 1;

        // Setup pipes
        if (!isLast && pipe(pipes[i % 2]) < 0) {
            perror("pipe");
            return;
        }

        pid_t pid = fork();
        if (pid == 0) { // Child process
            if (!isLast) {
                close(pipes[i % 2][0]); // Close read end
                dup2(pipes[i % 2][1], STDOUT_FILENO); // Write to pipe
            }
            if (i > 0) {
                close(pipes[(i - 1) % 2][1]); // Close write end of previous pipe
                dup2(in_fd, STDIN_FILENO); // Read from previous pipe
            }

            redirectIO(section);
            vector<char*> args = prepareArgs(section);
            execvp(args[0], args.data());
            cerr << "Command not found: " << args[0] << endl;
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork");
            return;
        } else { // Parent process
            if (!isLast) {
                close(pipes[i % 2][1]); // Close write end
                if (i > 0) close(in_fd); // Close previous read end
                in_fd = pipes[i % 2][0]; // Save read end for the next child
            }
            if (!inBackground || isLast) {
                int status;
                waitpid(pid, &status, 0); // Wait for the last process if not in background
            } else if (inBackground && isLast) {
                cout << "[" << pid << "]" << endl;
            }
        }
    }
}

vector<string> Shell::tokenize(const string& cmdLine) {
    stringstream ss(cmdLine);
    string token;
    vector<string> tokens;
    while (ss >> token) {
        for (size_t i = 0; i < token.length(); ++i) {
            char currentChar = token[i];
            // Check for special characters and operators.
        }
        tokens.push_back(token);
        // DEBUG( PRINT(__LOCATION__ << "tokenize" << " " << token << '\n') )
    }
    return tokens;
}

bool Shell::isBackgroundCommand(const vector<string>& tokens) {
    return !tokens.empty() && tokens.back() == "&";
}

vector<vector<string>> Shell::splitPipes(const vector<string>& tokens) {
    vector<vector<string>> pipeSections;
    vector<string> section;
    for (const auto& token : tokens) {
        if (token == "|") {
            if (!section.empty()) {
                pipeSections.push_back(section);
                section.clear();
            }
        } else {
            section.push_back(token);
        }
    }
    if (!section.empty()) {
        pipeSections.push_back(section);
    }
    return pipeSections;
}

void Shell::redirectIO(const vector<string>& cmdTokens) {
    for (size_t i = 0; i < cmdTokens.size(); ++i) {
        if (cmdTokens[i] == ">" || cmdTokens[i] == "<") {
            int fd = open(cmdTokens[i + 1].c_str(), cmdTokens[i] == ">" ? (O_WRONLY | O_CREAT | O_TRUNC) : O_RDONLY, S_IRUSR | S_IWUSR);
            if (fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, cmdTokens[i] == ">" ? STDOUT_FILENO : STDIN_FILENO);
            close(fd);
        }
    }
}

vector<char*> Shell::prepareArgs(const vector<string>& section) {
    vector<char*> args;
    for (const auto& arg : section) {
        args.push_back(const_cast<char*>(arg.c_str()));
    }
    args.push_back(nullptr);
    return args;
}
