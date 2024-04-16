/**
 * @author Qucheng Jiang / Shihan Zhao
 * @author jiang.qu@northeastern.edu / zhao.shiha@northeastern.edu
 * @author NUID 001569593 / 002772845
 */

#pragma once
#ifndef CSHELL_H
#define CSHELL_H

#include <string>
#include <vector>
#include "ast/CPipelineNode.h"

class CShell {
public:
    void run();
private:
    std::vector<std::string> tokenize(const std::string& cmdLine);
    bool isBackgroundCommand(const std::vector<std::string>& tokens);
    std::vector<std::vector<std::string>> splitPipes(const std::vector<std::string>& tokens);
    void executeCommand(const std::string& cmdLine);
    void executeCommand(const std::unique_ptr<CPipelineNode>& pipe);
    void redirectIO(ERedirectStdio_t redirect_fd, const std::string& redirect_file);
    void redirectIO(const std::vector<std::string>& cmdTokens);
    std::vector<char*> prepareArgs(const std::vector<std::string>& section);
};

#endif // CSHELL_H
