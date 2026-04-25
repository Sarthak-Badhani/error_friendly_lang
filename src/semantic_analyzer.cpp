#include "../include/semantic_analyzer.h"

SymbolTable::SymbolTable() : currentScope(0) {
    scopes.push_back(std::unordered_map<std::string, Symbol>());
}

void SymbolTable::enterScope() {
    currentScope++;
    if (scopes.size() <= static_cast<size_t>(currentScope)) {
        scopes.push_back(std::unordered_map<std::string, Symbol>());
    }
}

void SymbolTable::exitScope() {
    if (currentScope > 0) {
        scopes[currentScope].clear();
        currentScope--;
    }
}

void SymbolTable::addSymbol(const std::string& name, const std::string& type) {
    scopes[currentScope].insert({name, Symbol(name, type, currentScope)});
}

bool SymbolTable::lookupSymbol(const std::string& name) const {
    for (int i = currentScope; i >= 0; i--) {
        if (scopes[i].find(name) != scopes[i].end()) {
            return true;
        }
    }
    return false;
}

Symbol* SymbolTable::getSymbol(const std::string& name) {
    for (int i = currentScope; i >= 0; i--) {
        if (scopes[i].find(name) != scopes[i].end()) {
            return &scopes[i][name];
        }
    }
    return nullptr;
}

bool SymbolTable::isDeclaredInCurrentScope(const std::string& name) const {
    return scopes[currentScope].find(name) != scopes[currentScope].end();
}

SemanticAnalyzer::SemanticAnalyzer(std::shared_ptr<Program> program,
                                   std::shared_ptr<ErrorHandler> handler)
    : ast(program), errorHandler(handler) {}

void SemanticAnalyzer::analyze() {
    if (!ast) return;
    analyzeProgram(ast);
}

void SemanticAnalyzer::analyzeProgram(std::shared_ptr<Program> prog) {
    for (const auto& stmt : prog->statements) {
        analyzeStatement(stmt);
    }
}

void SemanticAnalyzer::analyzeStatement(StatementPtr stmt) {
    if (!stmt) return;
    
    if (auto varDecl = std::dynamic_pointer_cast<VarDeclaration>(stmt)) {
        // Check for redeclaration in current scope
        if (symbolTable.isDeclaredInCurrentScope(varDecl->name)) {
            errorHandler->addError(ErrorType::REDECLARED_VARIABLE,
                                 "Variable '" + varDecl->name + "' is already declared in this scope.",
                                 varDecl->line, varDecl->column,
                                 "Use a different variable name or declare it in an outer scope.");
        } else {
            symbolTable.addSymbol(varDecl->name, varDecl->type);
        }
        
        if (varDecl->initializer) {
            analyzeExpression(varDecl->initializer);

            // FIX #3: Type checking — compare declared type with initializer type
            std::string exprType = inferType(varDecl->initializer);
            if (exprType != "unknown") {
                bool typeMatch = false;

                if (varDecl->type == exprType) {
                    typeMatch = true;
                }
                // Allow int → float promotion
                else if (varDecl->type == "float" && exprType == "int") {
                    typeMatch = true;
                }

                if (!typeMatch) {
                    errorHandler->addError(ErrorType::TYPE_ERROR,
                        "Cannot assign '" + exprType + "' value to variable '" +
                        varDecl->name + "' of type '" + varDecl->type + "'.",
                        varDecl->line, varDecl->column,
                        "Make sure the value matches the declared type. "
                        "Expected '" + varDecl->type + "' but got '" + exprType + "'.");
                }
            }
        }
    } else if (auto exprStmt = std::dynamic_pointer_cast<ExpressionStatement>(stmt)) {
        analyzeExpression(exprStmt->expr);
    } else if (auto ifStmt = std::dynamic_pointer_cast<IfStatement>(stmt)) {
        analyzeExpression(ifStmt->condition);

        // FIX #4: Check that the if condition is a bool expression
        std::string condType = inferType(ifStmt->condition);
        if (condType != "unknown" && condType != "bool") {
            errorHandler->addError(ErrorType::TYPE_ERROR,
                "Condition in 'if' must be a boolean expression, but got '" + condType + "'.",
                ifStmt->condition->line, ifStmt->condition->column,
                "Use a comparison (e.g., x > 0) or a boolean variable.");
        }

        analyzeStatement(ifStmt->thenBranch);
        if (ifStmt->elseBranch) {
            analyzeStatement(ifStmt->elseBranch);
        }
    }
    // FIX #8: Handle BlockStatement — enter/exit scope
    else if (auto block = std::dynamic_pointer_cast<BlockStatement>(stmt)) {
        symbolTable.enterScope();
        for (const auto& s : block->statements) {
            analyzeStatement(s);
        }
        symbolTable.exitScope();
    }
}

void SemanticAnalyzer::analyzeExpression(ExpressionPtr expr) {
    if (!expr) return;
    
    if (auto var = std::dynamic_pointer_cast<Variable>(expr)) {
        if (!symbolTable.lookupSymbol(var->name)) {
            errorHandler->addError(ErrorType::UNDECLARED_VARIABLE,
                                 "Variable '" + var->name + "' is not declared.",
                                 var->line, var->column,
                                 "Declare the variable before using it: int " + var->name + ";");
        }
    } else if (auto binOp = std::dynamic_pointer_cast<BinaryOp>(expr)) {
        analyzeExpression(binOp->left);
        analyzeExpression(binOp->right);
    } else if (auto unOp = std::dynamic_pointer_cast<UnaryOp>(expr)) {
        analyzeExpression(unOp->operand);
    }
}

std::string SemanticAnalyzer::inferType(ExpressionPtr expr) {
    if (!expr) return "unknown";
    
    if (auto lit = std::dynamic_pointer_cast<Literal>(expr)) {
        switch (lit->value.type) {
            case TokenType::INT_LITERAL: return "int";
            case TokenType::FLOAT_LITERAL: return "float";
            case TokenType::STRING_LITERAL: return "string";
            case TokenType::KW_TRUE:
            case TokenType::KW_FALSE: return "bool";
            default: return "unknown";
        }
    }
    
    if (auto var = std::dynamic_pointer_cast<Variable>(expr)) {
        Symbol* sym = symbolTable.getSymbol(var->name);
        return sym ? sym->type : "unknown";
    }

    // FIX #4: Infer type for binary operators — comparisons produce bool
    if (auto binOp = std::dynamic_pointer_cast<BinaryOp>(expr)) {
        TokenType opType = binOp->op.type;

        // Comparison and logical operators produce bool
        if (opType == TokenType::OP_EQ || opType == TokenType::OP_NE ||
            opType == TokenType::OP_LT || opType == TokenType::OP_LE ||
            opType == TokenType::OP_GT || opType == TokenType::OP_GE ||
            opType == TokenType::OP_AND || opType == TokenType::OP_OR) {
            return "bool";
        }

        // Arithmetic operators — infer from operands
        std::string leftType = inferType(binOp->left);
        std::string rightType = inferType(binOp->right);

        if (leftType == "float" || rightType == "float") return "float";
        if (leftType == "int" && rightType == "int") return "int";
        if (leftType == "string" && rightType == "string") return "string";

        return leftType;  // Fallback
    }

    // Unary not produces bool
    if (auto unOp = std::dynamic_pointer_cast<UnaryOp>(expr)) {
        if (unOp->op.type == TokenType::OP_NOT) return "bool";
        return inferType(unOp->operand);
    }
    
    return "unknown";
}
