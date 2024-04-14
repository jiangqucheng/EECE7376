/**
 * @author Qucheng Jiang
 * @author jiang.qu@northeastern.edu
 * @author NUID 001569593
 */

#include "ast/CScriptNode.h"


void CScriptNode::addPipeline(std::unique_ptr<CPipelineNode> pipeline) 
{
    m_pipelines.push_back(std::move(pipeline));
}

const std::vector<std::unique_ptr<CPipelineNode>>& CScriptNode::getPipelines() const
{
    return m_pipelines;
}


std::ostream& operator<<(std::ostream& os, const CScriptNode& node) 
{
    os 
        << "[[[[[ --- SCRIPT --- ]]]]]" 
        << "\n    Contain " 
        << node.m_pipelines.size() 
        << " sep pipe.\n" 
        << std::endl
    ;
    for (const auto& pp : node.m_pipelines) 
        os << *pp << "\n";
    os << "[[[[[ --- SCRIPT END --- ]]]]]" << std::endl;
    return os;
}
