/**
 * @author Qucheng Jiang / Shihan Zhao
 * @author jiang.qu@northeastern.edu / zhao.shiha@northeastern.edu
 * @author NUID 001569593 / 002772845
 */

#pragma once

#include "CNode.h"
#include "CPipelineNode.h"

#include <iostream>
#include <vector>
// #include <sstream>
#include <memory>
// #include <algorithm>
// #include "debug.h"


class CScriptNode : CNode {
    // only one for now
    std::vector<std::unique_ptr<CPipelineNode>> m_pipelines;

public:
    void addPipeline(std::unique_ptr<CPipelineNode> pipeline);

    const std::vector<std::unique_ptr<CPipelineNode>>& getPipelines() const;

    friend std::ostream& operator<<(std::ostream& os, const CScriptNode& node);
};
