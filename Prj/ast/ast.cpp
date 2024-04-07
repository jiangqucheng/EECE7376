#include <iostream>
#include <vector>
#include <sstream>
#include <memory>
#include <algorithm>
#include "debug.h"

enum ERedirectStdio {REDIRECT_STDIN=0, REDIRECT_STDOUT=1, REDIRECT_STDERR=2, };
typedef enum ERedirectStdio ERedirectStdio_t;

// AST Node Types
class CNode {
public:
    virtual ~CNode() = default;
};

class CCommandNode : CNode {
    std::string                                             m_command;
    std::vector<std::string>                                m_arguments;
    std::vector<std::tuple<ERedirectStdio_t, std::string>>  m_redirects;
    bool                                                    m_background;

public:
    CCommandNode(std::string cmd)
    : m_command(std::move(cmd))
    , m_background(false)
    {}

    void setCommand(std::string cmd) {
        m_command = std::move(cmd);
    }

    void setBackground(bool background) {
        m_background = background;
    }

    void addArgument(std::string arg) {
        m_arguments.push_back(std::move(arg));
    }

    void redirect(ERedirectStdio_t redirect_fd, std::string redirect_file) {
        m_redirects.push_back(std::make_tuple(redirect_fd, redirect_file));
    }

    friend std::ostream& operator<<(std::ostream& os, const CCommandNode& node) {
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
};

class CPipelineNode : CNode {
    std::vector<std::unique_ptr<CCommandNode>> m_commands;

public:
    void addCommand(std::unique_ptr<CCommandNode> cmd) {
        m_commands.push_back(std::move(cmd));
    }

    friend std::ostream& operator<<(std::ostream& os, const CPipelineNode& node) {
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
};

class CScriptNode : CNode {
    // only one for now
    std::vector<std::unique_ptr<CPipelineNode>> m_pipelines;

public:
    void addPipeline(std::unique_ptr<CPipelineNode> pipeline) {
        m_pipelines.push_back(std::move(pipeline));
    }

    friend std::ostream& operator<<(std::ostream& os, const CScriptNode& node) {
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
};

// Parser
class PshParser {
public:
    std::unique_ptr<CScriptNode> parse(const std::string& input) {
        auto script = std::make_unique<CScriptNode>();
        auto pipeline = std::make_unique<CPipelineNode>();

        std::istringstream input_stream(input);
        
        // Tracks the current isInQuote context (0, '\'', '\"', '`')
        char isInQuote = 0; 
        // auto clearQuote = [&isInQuote]() { isInQuote = 0; };
        auto updateQuote = [&isInQuote](char q) { if (isInQuote == q) isInQuote = 0 /* end */; else if (isInQuote == 0) isInQuote = q /* start */; /* else -> ignore; */ };

        auto isQuoteSymbol =  [](char ch) -> bool {return (ch == '\'' || ch == '\"' || ch == '`');};
        auto isRedirectSymbol =  [](char ch) -> bool {return (ch == '>' || ch == '<');};
        auto isPipelineSymbol =  [](char ch) -> bool {return (ch == '|');};
        auto isCommandEndSymbol =  [](char ch) -> bool {return (ch == ';' || ch == '|');};
        auto isBackgroundSymbol =  [](char ch) -> bool {return (ch == '&');};

        bool isInCmd = true; // True if we're expecting a command, false for arguments
        bool isInRedirect = false; // True if we're expecting a redirect, false for arguments
        ERedirectStdio_t isRedirecting = REDIRECT_STDIN;
        std::string t_currentStr = "";

        
        // Tracks the current is part of command or arg
        auto currentCommandNode = std::make_unique<CCommandNode>("");
        auto updateCommandNode = [&currentCommandNode, &isInCmd, &isInRedirect, &isRedirecting](std::string &str) { 
            if (str.size() > 0) {
                if (isInCmd){
                    // in command
                    currentCommandNode->setCommand(std::move(str));
                    isInCmd = false;
                    isInRedirect = false;
                }
                else if (!isInRedirect) {
                    // in args
                    currentCommandNode->addArgument(std::move(str));
                }
                else {
                    // in Redirect
                    currentCommandNode->redirect(isRedirecting, std::move(str));\
                    isInRedirect = false;
                    isRedirecting = REDIRECT_STDIN;
                }
                str = "";
            }
        };

        for (auto ch : input ) 
        {
            if (isQuoteSymbol(ch)) {updateQuote(ch);}
            t_currentStr += ch;

            // skip in still in quote text mode
            if (isInQuote) continue;

            if (!isInRedirect && isRedirectSymbol(ch)) 
            {
                if ("2>" == t_currentStr) 
                {
                    isRedirecting = REDIRECT_STDERR;
                    // we are sure that redirect to stderr must have a space in front.
                    t_currentStr = "";
                }
                else
                {
                    t_currentStr.pop_back();
                    // redirect to stdin/stdout may not have space in front. 
                    // update that part first.
                    updateCommandNode(t_currentStr);
                    isRedirecting = (ch == '>') ? REDIRECT_STDOUT : REDIRECT_STDIN;
                }
                isInRedirect = true;
            }

            if (isspace(ch) || isCommandEndSymbol(ch) || (isBackgroundSymbol(ch) && (!isInRedirect || t_currentStr.size()>=2)))
            {
                t_currentStr.pop_back();
                updateCommandNode(t_currentStr);
                
                if ((isBackgroundSymbol(ch) && !isInRedirect)) currentCommandNode->setBackground(true);

                if (isCommandEndSymbol(ch))
                {
                    pipeline->addCommand(std::move(currentCommandNode));
                    if (!isPipelineSymbol(ch))
                    {
                        script->addPipeline(std::move(pipeline));
                        // get a new pipeline container
                        pipeline = std::make_unique<CPipelineNode>();
                    }
                    // get a new command container
                    currentCommandNode = std::make_unique<CCommandNode>("");
                    isInCmd = true;
                    isInRedirect = false;
                }
            }
        }

        // Process any remaining command and arguments
        updateCommandNode(t_currentStr);
        // update pipeline list
        pipeline->addCommand(std::move(currentCommandNode));
        // update script list
        script->addPipeline(std::move(pipeline));

        return script;
    }
};

// Main function for demonstration
int main(int argc, char* argv[])
{
    std::string argStr;
    // start from id=1 since argv[0] is executable name.
    for (int i = 1; i < argc; ++i) argStr += std::string(argv[i]) + ((i < argc-1) ? " " : ""); 

    PshParser parser;
    auto ast = parser.parse(argStr);

    std::cout << *ast;

    return 0;
}
