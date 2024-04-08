#pragma once
#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <vector>

class Shell {
public:
    void run();
private:
    std::vector<std::string> tokenize(const std::string& cmdLine);
    bool isBackgroundCommand(const std::vector<std::string>& tokens);
    std::vector<std::vector<std::string>> splitPipes(const std::vector<std::string>& tokens);
    void executeCommand(const std::string& cmdLine);
    void redirectIO(const std::vector<std::string>& cmdTokens);
    std::vector<char*> prepareArgs(const std::vector<std::string>& section);
};

#endif // SHELL_H
