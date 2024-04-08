#include "ast/CCommandNode.h"

CCommandNode::CCommandNode(std::string cmd)
: m_command(std::move(cmd))
, m_background(false)
{}


const std::string& CCommandNode::getCommand() const
{
    return m_command;
}
bool CCommandNode::isBackground() const
{
    return m_background;
}
const std::vector<std::string>& CCommandNode::getArgument() const
{
    return m_arguments;
}
const std::vector<std::tuple<ERedirectStdio_t, std::string>>& CCommandNode::getRedirects() const
{
    return m_redirects;
}


void CCommandNode::setCommand(std::string cmd) 
{
    m_command = std::move(cmd);
}

void CCommandNode::setBackground(bool background) 
{
    m_background = background;
}

void CCommandNode::addArgument(std::string arg) 
{
    m_arguments.push_back(std::move(arg));
}

void CCommandNode::redirect(ERedirectStdio_t redirect_fd, std::string redirect_file) 
{
    m_redirects.push_back(std::make_tuple(redirect_fd, redirect_file));
}

std::ostream& operator<<(std::ostream& os, const CCommandNode& node) 
{
    std::size_t ii = 0;
    os << "CMD: " << node.m_command << (node.m_background?"  (BG)":"") << std::endl;
    os << "ARG: [";
    ii = 0;
    for (const auto& arg : node.m_arguments) 
    {
        os
            << arg
            << (((ii++)<node.m_arguments.size()-1)?",":"")
        ;
    }
    os << "]";
    os << std::endl;
    os << "IOE: [";
    ii = 0;
    for (const auto& redirect : node.m_redirects) 
    {
        os
            << std::get<ERedirectStdio_t>(redirect)
            << "="
            << std::get<std::string>(redirect)
            << (((ii++)<node.m_redirects.size()-1)?",":"")
        ;
    }
    os << "]";
    os << std::endl;
    return os;
}
