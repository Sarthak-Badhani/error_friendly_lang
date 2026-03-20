#ifndef COMPILER_H
#define COMPILER_H

#include "lexer.h"
#include "parser.h"
#include "semantic_analyzer.h"
#include "error_handler.h"
#include <memory>
#include <string>

class Compiler {
private:
    std::string sourceCode;
    std::shared_ptr<ErrorHandler> errorHandler;
    std::shared_ptr<Program> ast;
    std::vector<Token> tokens;
    
public:
    Compiler();
    
    bool compile(const std::string& source);
    bool compileFromFile(const std::string& filename);
    
    void displayResults();
    bool hasErrors() const;
    
private:
    std::string readFile(const std::string& filename);
};

#endif
