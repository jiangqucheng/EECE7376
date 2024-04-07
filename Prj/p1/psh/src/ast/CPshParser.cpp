#include "ast/CPshParser.h"

// Parser
std::unique_ptr<CScriptNode> CPshParser::parse(const std::string& input) 
{
    auto script = std::make_unique<CScriptNode>();
    auto pipeline = std::make_unique<CPipelineNode>();

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
