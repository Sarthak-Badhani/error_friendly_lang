#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "parser.h"
#include "error_handler.h"
#include <memory>
#include <unordered_map>
#include <vector>

struct Symbol {
    std::string name;
    std::string type;
    int scope;
    bool initialized;
    
    Symbol() : name(""), type(""), scope(0), initialized(false) {}
    Symbol(const std::string& n, const std::string& t, int s, bool init = false)
        : name(n), type(t), scope(s), initialized(init) {}
};

class SymbolTable {
private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes;
    int currentScope;
    
public:
    SymbolTable();
    
    void enterScope();
    void exitScope();
    
    void addSymbol(const std::string& name, const std::string& type);
    bool lookupSymbol(const std::string& name) const;
    Symbol* getSymbol(const std::string& name);
    bool isDeclaredInCurrentScope(const std::string& name) const;
};

class SemanticAnalyzer {
private:
    std::shared_ptr<Program> ast;
    std::shared_ptr<ErrorHandler> errorHandler;
    SymbolTable symbolTable;
    
    void analyzeProgram(std::shared_ptr<Program> prog);
    void analyzeStatement(StatementPtr stmt);
    void analyzeExpression(ExpressionPtr expr);
    
    std::string inferType(ExpressionPtr expr);
    
public:
    SemanticAnalyzer(std::shared_ptr<Program> program,
                     std::shared_ptr<ErrorHandler> handler);
    
    void analyze();
};

#endif
