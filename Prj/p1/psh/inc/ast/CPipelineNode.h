/**
 * @author Qucheng Jiang / Shihan Zhao
 * @author jiang.qu@northeastern.edu / zhao.shiha@northeastern.edu
 * @author NUID 001569593 / 002772845
 */

#pragma once

#include "CNode.h"
#include "CCommandNode.h"

#include <iostream>
#include <vector>
#include <memory>

class CPipelineNode : CNode {
    std::vector<std::unique_ptr<CCommandNode>> m_commands;

public:
    void addCommand(std::unique_ptr<CCommandNode> cmd);
    
    const std::vector<std::unique_ptr<CCommandNode>>& getCommands() const;

    friend std::ostream& operator<<(std::ostream& os, const CPipelineNode& node);
};
