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
