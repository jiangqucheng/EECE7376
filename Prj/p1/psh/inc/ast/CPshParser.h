/**
 * @author Qucheng Jiang
 * @author jiang.qu@northeastern.edu
 * @author NUID 001569593
 */

#pragma once

#include "CScriptNode.h"
#include <memory>

// Parser
class CPshParser 
{
public:
    std::unique_ptr<CScriptNode> parse(const std::string& input);
};
