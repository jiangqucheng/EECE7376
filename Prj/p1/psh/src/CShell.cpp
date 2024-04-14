/**
 * @author Qucheng Jiang
 * @author jiang.qu@northeastern.edu
 * @author NUID 001569593
 */

#include "debug.h"
#include "CShell.h"
#include "ast/CPshParser.h"
#include "ast/CPipelineNode.h"
#include "ast/CCommandNode.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>

using namespace std;

void CShell::run() {
    std::string cmdLine;
    CPshParser parser;
    std::unique_ptr<CScriptNode> p_command_ast;

    while (true) {
        PRINT("$ ");
        getline(std::cin, cmdLine);
        if (cin.eof()) break;
        
        p_command_ast = parser.parse(cmdLine);
        for (auto& pipe : p_command_ast->getPipelines()) 
            executeCommand(pipe);
    }
}

void CShell::executeCommand(const std::unique_ptr<CPipelineNode> &current_pipeline_commands) 
{
    int pipes[2][2], in_fd = 0;
    size_t cmd_id = 0;
    bool isLastCmd;

    DEBUG(PRINT(__LOCATION__ << std::endl))

    for(auto& cmd: current_pipeline_commands->getCommands())
    {
        isLastCmd = cmd_id == current_pipeline_commands->getCommands().size() - 1;
        // for each command
        DEBUG(PRINT(__LOCATION__ << isLastCmd << std::endl << *cmd))
        // Setup pipes
        if (pipe(pipes[cmd_id % 2]) < 0) { perror("pipe"); exit(1); }  // exit if pipe not successfully created

        pid_t pid = fork(); 
        if (pid < 0) { perror("fork");exit(1); }  // exit if pipe not successfully created
        switch (pid)
        {
        case 0:  // child
        {
            if (!isLastCmd) {
                close(pipes[cmd_id % 2][0]); // Close read end
                dup2(pipes[cmd_id % 2][1], STDOUT_FILENO); // Write to pipe
            }
            if (cmd_id > 0) {
                close(pipes[(cmd_id - 1) % 2][1]); // Close write end of previous pipe
                dup2(in_fd, STDIN_FILENO); // Read from previous pipe
            }

            for(auto& rdct_info: cmd->getRedirects())
                redirectIO(std::get<ERedirectStdio_t>(rdct_info), std::get<std::string>(rdct_info));
            
            std::vector<char*> args = prepareArgs(cmd->getArgument());
            args.insert(args.begin(), const_cast<char*>(cmd->getCommand().c_str()));
            execvp(args[0], args.data());
            // Executable should take over this child process
            // Continue if error occurs.
            cerr << "Command not found: " << args[0] << endl;
            exit(EXIT_FAILURE);
        }
            break;
        default:  // parent
        {
            if (!isLastCmd) {
                close(pipes[cmd_id % 2][1]); // Parent should Close write end, as this is the pipe from current child to next child.
                if (cmd_id > 0) close(in_fd); // Close previous read end
                in_fd = pipes[cmd_id % 2][0]; // Save read end for the next child
            }

            if (!cmd->isBackground()) 
            {
                int status;
                waitpid(pid, &status, 0); // Wait for the last process if not in background
            }
            else PRINT("[" << pid << "]" << std::endl)
        }
            break;
        }
        cmd_id++;
    }
    PRINT("\n")
}

void CShell::redirectIO(ERedirectStdio_t redirect_fd, const std::string& redirect_file) 
{
    int fd;
    switch (redirect_fd)
    {
    case REDIRECT_STDIN:
        fd = open(redirect_file.c_str(), O_RDONLY, S_IRUSR | S_IWUSR);
        break;
    case REDIRECT_STDOUT:
    case REDIRECT_STDERR:
        if (redirect_file.front() == '&')
            fd = redirect_file[1] - '0';
        else
            fd = open(redirect_file.c_str(), (O_WRONLY | O_CREAT | O_TRUNC), S_IRUSR | S_IWUSR);
        break;
    default:
        perror("redirectIO");
        exit(1);
        break;
    }
    if (fd < 0) { perror("fd open"); exit(EXIT_FAILURE); }  // exit if file descriptor create err
    // swap the new added fd to redirect_fd
    dup2(fd, redirect_fd);
    if (fd > STDOUT_FILENO) close(fd);
}





std::vector<char*> CShell::prepareArgs(const std::vector<std::string>& section) {
    std::vector<char*> args;
    for (const auto& arg : section) {
        args.push_back(const_cast<char*>(arg.c_str()));
    }
    args.push_back(nullptr);
    return args;
}





void CShell::executeCommand(const string& cmdLine) {
    auto tokens = tokenize(cmdLine);
    bool inBackground = isBackgroundCommand(tokens);
    if (inBackground) {
        tokens.pop_back(); // Remove the "&"
    }
    auto pipeSections = splitPipes(tokens);

    DEBUG
    (
        PRINT(__LOCATION__ << std::endl)
        for(auto cmd: pipeSections)
        {
            PRINT(__LOCATION__ << std::endl)
            for(auto arg: cmd) PRINT("   " << arg << "   ")
            PRINT("\n")
        }
        PRINT("\n")
    )

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

vector<string> CShell::tokenize(const string& cmdLine) {
    stringstream ss(cmdLine);
    string token;
    vector<string> tokens;
    while (ss >> token) {
        for (size_t i = 0; i < token.length(); ++i) {
            // char currentChar = token[i];
            // Check for special characters and operators.
        }
        tokens.push_back(token);
        // DEBUG( PRINT(__LOCATION__ << "tokenize" << " " << token << '\n') )
    }
    return tokens;
}

bool CShell::isBackgroundCommand(const vector<string>& tokens) {
    return !tokens.empty() && tokens.back() == "&";
}

vector<vector<string>> CShell::splitPipes(const vector<string>& tokens) {
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

void CShell::redirectIO(const vector<string>& cmdTokens) {
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
