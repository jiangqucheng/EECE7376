#include <iostream>
#include <vector>
#include <sstream>
#include <memory>
#include <algorithm>
#include "debug.h"

// AST Node Types
struct Node {
    virtual ~Node() = default;
};

struct CommandNode : Node {
    std::string                 m_command;
    std::vector<std::string>    m_arguments;
    std::array<std::string, 3>  m_redirects;

    CommandNode(std::string cmd)
    : m_command(std::move(cmd))
    , m_redirects({"", "", ""})
    {}

    void setCommand(std::string cmd) {
        m_command = std::move(cmd);
    }

    void addArgument(std::string arg) {
        m_arguments.push_back(std::move(arg));
    }

    void redirect(uint index, std::string redirect_file) {
        m_redirects[index] = std::move(redirect_file);
    }

    friend std::ostream& operator<<(std::ostream& os, const CommandNode& node) {
        os << "Command: " << node.m_command << std::endl;
        os << "Arguments: [";
        for (const auto& arg : node.m_arguments) {
            os << arg << ",";
        }
        os << "]";
        os << std::endl;
        os << "Redirects: [";
        for (const auto& redirect : node.m_redirects) {
            os << redirect << ",";
        }
        os << "]";
        os << std::endl;
        return os;
    }
};

struct PipelineNode : Node {
    std::vector<std::unique_ptr<CommandNode>> m_commands;

    void addCommand(std::unique_ptr<CommandNode> cmd) {
        m_commands.push_back(std::move(cmd));
    }

    friend std::ostream& operator<<(std::ostream& os, const PipelineNode& node) {
        os << "pipeline stages: " << node.m_commands.size() << std::endl;
        os << "---------" << std::endl;
        for (const auto& cmd : node.m_commands) {
            os << *cmd;
            os << "- - - - -" << std::endl;
        }
        os << "---------" << std::endl;
        os << std::endl;
        return os;
    }
};

struct ScriptNode : Node {
    // only one for now
    std::vector<std::unique_ptr<PipelineNode>> pipelines;

    void addPipeline(std::unique_ptr<PipelineNode> pipeline) {
        pipelines.push_back(std::move(pipeline));
    }
};

// Parser
class BashParser {
public:
    std::unique_ptr<ScriptNode> parse(const std::string& input) {
        auto script = std::make_unique<ScriptNode>();
        auto pipeline = std::make_unique<PipelineNode>();

        std::istringstream input_stream(input);
        char ch;
        
        // Tracks the current isInQuote context (0, '\'', '\"', '`')
        char isInQuote = 0; 
        auto clearQuote = [&isInQuote]() { isInQuote = 0; };
        auto updateQuote = [&isInQuote](char q) { if (isInQuote == q) isInQuote = 0 /* end */; else if (isInQuote == 0) isInQuote = q /* start */; /* else -> ignore; */ };

        auto isQuoteSymbol =  [](char ch) -> bool {return (ch == '\'' || ch == '\"' || ch == '`');};
        auto isRedirectSymbol =  [](char ch) -> bool {return (ch == '>' || ch == '<');};
        auto isCommandEndSymbol =  [](char ch) -> bool {return (ch == '&' || ch == '|');};

        bool isInCmd = true; // True if we're expecting a command, false for arguments
        bool isInRedirect = false; // True if we're expecting a redirect, false for arguments
        int isRedirecting = 0; // 0 - stdin; 1 - stdout; 2 - stderr; 
        std::string t_currentStr = "";


        
        // Tracks the current is part of command or arg
        auto currentCommandNode = std::make_unique<CommandNode>("");
        auto updateCommandNode = [&currentCommandNode, &isInCmd, &isInRedirect, &isRedirecting](std::string str) { 
            if (str.size() > 0) {
                if (isInCmd){
                    currentCommandNode->setCommand(std::move(str));
                    isInCmd = false;
                    isInRedirect = false;
                }
                else if (!isInRedirect) {
                    // in args
                    if (str.size() > 0) currentCommandNode->addArgument(std::move(str));
                }
                else {
                    // in Redirect
                    // PRINT(__LOCATION__ << str << '\n' )
                    currentCommandNode->redirect(isRedirecting, std::move(str));\
                    isRedirecting = 0;
                    isInRedirect = false;
                }
            }
        };

        for (auto ch : input ) 
        {
            // PRINT(__LOCATION__ << ch << '\n')
            if (isQuoteSymbol(ch)) {updateQuote(ch);}
            t_currentStr += ch;

            if (isInQuote) continue;

            if (!isInRedirect && isRedirectSymbol(ch)) 
            {
                if ("2>" == t_currentStr) isRedirecting = 2;
                else
                {
                    t_currentStr.pop_back();
                    updateCommandNode(t_currentStr);
                    if (ch == '>') isRedirecting = 1; else isRedirecting = 0;
                }
                t_currentStr = "";
                isInRedirect=true;
            }

            if (isspace(ch) || (isCommandEndSymbol(ch) && !(isInRedirect && ch=='&')))
            {
                t_currentStr.pop_back();
                updateCommandNode(t_currentStr);
                t_currentStr = "";
                
                if (isCommandEndSymbol(ch))
                {
                    pipeline->addCommand(std::move(currentCommandNode));
                    currentCommandNode = std::make_unique<CommandNode>("");
                    isInCmd = true;
                    isInRedirect = false;
                }
            }
        }

        updateCommandNode(t_currentStr);

        pipeline->addCommand(std::move(currentCommandNode));
        script->addPipeline(std::move(pipeline));

        // std::string currentCmd;
        // std::vector<std::string> currentArgs;
        // bool isInCmd = true; // True if we're expecting a command, false for arguments

        // while (input_stream >> ch) 
        // {
        //     // PRINT(__LOCATION__ << token << '\n')
        //     for (char ch : token)
        //     {
        //         if (isCharAQuoteSymbol(ch) && (isInQuote == 0 || isInQuote == ch)) {
        //             setQuote(ch); // Start or end isInQuote context
        //         }

        //         if (isInQuote || !isspace(ch)) { // Inside quotes or non-space character
        //             (isInCmd ? currentCmd : currentArgs.back()) += ch;
        //         } else if (!isInCmd && isspace(ch)) { // Argument separator outside quotes
        //             currentArgs.push_back(""); // Start a new argument
        //         }
        //     }

        //     // PRINT(__LOCATION__ << "[" << (isInCmd ? "Cmd" : "Arg") << "]" << " {" << currentCmd << "} Arg:" << (currentArgs.size() ? currentArgs.front() : "") << '\n')

        //     if (!isInQuote) { // Not inside quotes
        //         if (isInCmd) {
        //             isInCmd = false; // Next tokens will be arguments
        //         }
        //         currentArgs.push_back(""); // Prepare for the first argument
        //     }

        //     // Handle special case if the last character was a separator (like '|')
        //     if (!isspace(token.back()) && isInQuote == 0) {
        //         if (token.back() == '|') {
        //             // Process current command and arguments
        //             auto pipeline = std::make_unique<PipelineNode>();
        //             auto commandNode = std::make_unique<CommandNode>(currentCmd);
        //             commandNode->arguments = std::move(currentArgs);
        //             pipeline->addCommand(std::move(commandNode));
        //             script->addPipeline(std::move(pipeline));

        //             // Reset for next command
        //             currentCmd.clear();
        //             currentArgs.clear();
        //             isInCmd = true;
        //         } else {
        //             // currentArgs.back() += token.back();
        //         }
        //     }
        // }

        // // Process any remaining command and arguments
        // if (!currentCmd.empty()) {
        //     auto pipeline = std::make_unique<PipelineNode>();
        //     auto commandNode = std::make_unique<CommandNode>(currentCmd);
        //     commandNode->arguments = std::move(currentArgs);
        //     pipeline->addCommand(std::move(commandNode));
        //     script->addPipeline(std::move(pipeline));
        // }

        return script;
    }
};

// Main function for demonstration
int main() {
    BashParser parser;
    // auto ast = parser.parse("echo \"Hello, world\" -e aaa | grep 'world'");
    auto ast = parser.parse("echo \"Hello, world\"     <    hhhass?c -e aaa> adsnii 2>&1|grep 'world' ");

    // Demonstration: print the parsed command
    for (const auto& pipeline : ast->pipelines) {
        std::cout << *pipeline;
        std::cout << "\n";
    }

    return 0;
}
