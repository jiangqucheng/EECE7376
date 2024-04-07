#include <iostream>
#include <string>
#include "ast/CPshParser.h"


// Main function for demonstration
int main(int argc, char* argv[])
{
    std::string argStr;
    // start from id=1 since argv[0] is executable name.
    for (int i = 1; i < argc; ++i) argStr += std::string(argv[i]) + ((i < argc-1) ? " " : ""); 

    CPshParser parser;
    auto ast = parser.parse(argStr);

    std::cout << *ast;

    return 0;
}
