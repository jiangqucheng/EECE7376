/**
 * @author Qucheng Jiang / Shihan Zhao
 * @author jiang.qu@northeastern.edu / zhao.shiha@northeastern.edu
 * @author NUID 001569593 / 002772845
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
