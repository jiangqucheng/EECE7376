#include "ast/CScriptNode.h"


void CScriptNode::addPipeline(std::unique_ptr<CPipelineNode> pipeline) 
{
    m_pipelines.push_back(std::move(pipeline));
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
