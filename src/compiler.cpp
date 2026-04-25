#include "../include/compiler.h"
#include <fstream>
#include <iostream>

Compiler::Compiler() : errorHandler(std::make_shared<ErrorHandler>()) {}

std::string Compiler::readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'" << std::endl;
        return "";
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();
    return content;
}

bool Compiler::compile(const std::string& source) {
    sourceCode = source;
    errorHandler->clearErrors();
    
    // Stage 1: Lexical Analysis
    std::cout << "=== Lexical Analysis ===" << std::endl;
    Lexer lexer(sourceCode, errorHandler);
    tokens = lexer.tokenize();
    
    if (errorHandler->hasAnyErrors()) {
        std::cout << "Lexical analysis failed." << std::endl;
        // FIX #1: Do NOT call reportAllErrors() here — let displayResults() do it once.
        return false;
    }
    std::cout << "✓ Lexical analysis completed successfully." << std::endl;
    std::cout << "  Tokens generated: " << tokens.size() << std::endl << std::endl;
    
    // Stage 2: Syntax Analysis
    std::cout << "=== Syntax Analysis ===" << std::endl;
    Parser parser(tokens, errorHandler);
    ast = parser.parse();
    
    if (errorHandler->hasAnyErrors()) {
        std::cout << "Syntax analysis failed." << std::endl;
        // FIX #1: Do NOT call reportAllErrors() here.
        return false;
    }
    std::cout << "✓ Syntax analysis completed successfully." << std::endl;
    std::cout << "  AST nodes created: " << ast->statements.size() << std::endl << std::endl;
    
    // Stage 3: Semantic Analysis
    std::cout << "=== Semantic Analysis ===" << std::endl;
    SemanticAnalyzer semAnalyzer(ast, errorHandler);
    semAnalyzer.analyze();
    
    if (errorHandler->hasAnyErrors()) {
        std::cout << "Semantic analysis failed." << std::endl;
        // FIX #1: Do NOT call reportAllErrors() here.
        return false;
    }
    std::cout << "✓ Semantic analysis completed successfully." << std::endl << std::endl;
    
    return true;
}

bool Compiler::compileFromFile(const std::string& filename) {
    std::string source = readFile(filename);
    if (source.empty()) {
        return false;
    }
    return compile(source);
}

void Compiler::displayResults() {
    if (hasErrors()) {
        // FIX #1/#2: This is now the ONLY place errors are printed,
        // so each error appears exactly once and the count matches.
        errorHandler->reportAllErrors();
    } else {
        std::cout << "=== COMPILATION SUCCESSFUL ===" << std::endl;
        std::cout << " No errors found! Your program is valid." << std::endl;
    }
}

bool Compiler::hasErrors() const {
    return errorHandler->hasAnyErrors();
}
