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

    friend std::ostream& operator<<(std::ostream& os, const CPipelineNode& node);
};
