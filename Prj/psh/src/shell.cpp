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
    string input;
    pid_t pid, wpid;
    int status;

    while (true) {
        cout << "$ ";
        getline(cin, input);

        if (cin.eof()) break; // Exit on EOF

        stringstream ss(input);
        vector<string> args;
        string arg;
        bool background = false;

        while (ss >> arg) {
            if (arg == "&") {
                background = true;
            } else {
                args.push_back(arg);
            }
        }

        if (args.empty()) continue; // Empty command

        vector<char*> c_args;
        for (auto &arg : args) c_args.push_back(&arg[0]);
        c_args.push_back(nullptr);

        // Check for redirection operators and set flags
        int in_fd = -1, out_fd = -1;
        for (std::size_t i = 0; i < args.size(); ++i) {
            if (args[i] == "<") {
                in_fd = open(args[i + 1].c_str(), O_RDONLY);
                if (in_fd < 0) {
                    cerr << args[i + 1] << ": File not found" << endl;
                    goto next_prompt;
                }
                c_args.erase(c_args.begin() + i, c_args.begin() + i + 2);
                break;
            } else if (args[i] == ">") {
                out_fd = open(args[i + 1].c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (out_fd < 0) {
                    cerr << args[i + 1] << ": Cannot create file" << endl;
                    goto next_prompt;
                }
                c_args.erase(c_args.begin() + i, c_args.begin() + i + 2);
                break;
            }
        }

        pid = fork();
        if (pid == 0) {
            // Child process
            if (in_fd >= 0) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if (out_fd >= 0) {
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            }

            if (execvp(c_args[0], c_args.data()) == -1) {
                cerr << c_args[0] << ": Command not found" << endl;
            }
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            // Error forking
            perror("fork");
        } else {
            // Parent process
            if (!background) {
                do {
                    wpid = waitpid(pid, &status, WUNTRACED);
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            } else {
                cout << "[" << pid << "]" << endl;
            }
        }

    next_prompt:
        if (in_fd >= 0) close(in_fd);
        if (out_fd >= 0) close(out_fd);
    }
}
