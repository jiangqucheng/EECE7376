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

    void setBackground(bool background);

    void addArgument(std::string arg);

    void redirect(ERedirectStdio_t redirect_fd, std::string redirect_file);

    friend std::ostream& operator<<(std::ostream& os, const CCommandNode& node);
};
