/**
 * @author Qucheng Jiang
 * @author jiang.qu@northeastern.edu
 * @author NUID 001569593
 */

#pragma once

#include "CNode.h"
#include "ERedirectStdio.h"

#include <iostream>
#include <vector>
#include <tuple>

class CCommandNode : CNode {
    std::string                                             m_command;
    std::vector<std::string>                                m_arguments;
    std::vector<std::tuple<ERedirectStdio_t, std::string>>  m_redirects;
    bool                                                    m_background;

public:
    CCommandNode(std::string cmd);

    void setCommand(std::string cmd);
    const std::string& getCommand() const;

    void setBackground(bool background);
    bool isBackground() const;

    void addArgument(std::string arg);
    const std::vector<std::string>& getArgument() const;

    void redirect(ERedirectStdio_t redirect_fd, std::string redirect_file);
    const std::vector<std::tuple<ERedirectStdio_t, std::string>>& getRedirects() const;

    friend std::ostream& operator<<(std::ostream& os, const CCommandNode& node);
};

