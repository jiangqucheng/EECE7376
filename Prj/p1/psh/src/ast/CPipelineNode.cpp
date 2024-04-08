#include "ast/CPipelineNode.h"

void CPipelineNode::addCommand(std::unique_ptr<CCommandNode> cmd)
{
    m_commands.push_back(std::move(cmd));
}

const std::vector<std::unique_ptr<CCommandNode>>& CPipelineNode::getCommands() const
{
    return m_commands;
}

std::ostream& operator<<(std::ostream& os, const CPipelineNode& node)
{
    os
        << "------------------------"
        << "<"
        << node.m_commands.size()
        << ">"
        << std::endl
    ;
    std::size_t ii = 0;
    for (const auto& cmd : node.m_commands) 
        os
            << *cmd 
            << (((ii++)<node.m_commands.size()-1)?"- - - - - - - - - - - - - -\n":"")
        ;
    os << "---------------------------" << std::endl;
    return os;
}
