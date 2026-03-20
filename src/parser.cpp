#include "../include/parser.h"
#include <iostream>

Parser::Parser(const std::vector<Token>& toks, std::shared_ptr<ErrorHandler> handler)
    : tokens(toks), current(0), errorHandler(handler) {}

Token Parser::peek() const {
    return tokens[current];
}

Token Parser::previous() const {
    return tokens[current - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    
    Token current = peek();
    errorHandler->addError(ErrorType::SYNTAX_ERROR, message, 
                          current.line, current.column);
    return current;
}

void Parser::synchronize() {
    advance();
    
    while (!isAtEnd()) {
        switch (peek().type) {
            case TokenType::KW_IF:
            case TokenType::KW_FOR:
            case TokenType::KW_WHILE:
            case TokenType::KW_INT:
            case TokenType::KW_FLOAT:
            case TokenType::KW_BOOL:
            case TokenType::KW_STRING:
            case TokenType::KW_RETURN:
                return;
            default:
                advance();
        }
    }
}

StatementPtr Parser::statement() {
    if (check(TokenType::LBRACE)) {
        // Block statement - simplified
        advance();
        while (!check(TokenType::RBRACE) && !isAtEnd()) {
            statement();
        }
        if (!check(TokenType::RBRACE)) {
            errorHandler->addError(ErrorType::SYNTAX_ERROR,
                                 "Expected '}' to close block", 
                                 peek().line, peek().column,
                                 "Make sure all opening braces '{' have a closing '}'");
        }
        consume(TokenType::RBRACE, "Expected '}' after block");
        return nullptr;
    }
    
    if (match(std::vector<TokenType>{TokenType::KW_IF})) {
        // If statement - simplified implementation
        consume(TokenType::LPAREN, "Expected '(' after 'if'");
        ExpressionPtr condition = expression();
        consume(TokenType::RPAREN, "Expected ')' after condition");
        StatementPtr thenBranch = statement();
        StatementPtr elseBranch = nullptr;
        if (match(std::vector<TokenType>{TokenType::KW_ELSE})) {
            elseBranch = statement();
        }
        return std::make_shared<IfStatement>(condition, thenBranch, elseBranch,
                                            peek().line, peek().column);
    }
    
    // Expression statement
    ExpressionPtr expr = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression. Did you forget the semicolon?");
    return std::make_shared<ExpressionStatement>(expr, peek().line, peek().column);
}

StatementPtr Parser::declaration() {
    if (check(TokenType::KW_INT) || check(TokenType::KW_FLOAT) ||
        check(TokenType::KW_BOOL) || check(TokenType::KW_STRING)) {
        return varDeclaration();
    }
    
    return statement();
}

StatementPtr Parser::varDeclaration() {
    Token typeToken = advance();
    std::string type = typeToken.lexeme;
    
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name after type");
    
    ExpressionPtr initializer = nullptr;
    if (match(std::vector<TokenType>{TokenType::OP_ASSIGN})) {
        initializer = expression();
    }
    
    consume(TokenType::SEMICOLON, "Expected ';' after variable declaration");
    return std::make_shared<VarDeclaration>(name.lexeme, type, initializer,
                                           name.line, name.column);
}

ExpressionPtr Parser::expression() {
    return assignment();
}

ExpressionPtr Parser::assignment() {
    ExpressionPtr expr = logicalOr();
    
    if (match(std::vector<TokenType>{TokenType::OP_ASSIGN})) {
        Token equals = previous();
        ExpressionPtr value = assignment();
        return std::make_shared<BinaryOp>(expr, equals, value,
                                         equals.line, equals.column);
    }
    
    return expr;
}

ExpressionPtr Parser::logicalOr() {
    ExpressionPtr expr = logicalAnd();
    
    while (match(std::vector<TokenType>{TokenType::OP_OR})) {
        Token op = previous();
        ExpressionPtr right = logicalAnd();
        expr = std::make_shared<BinaryOp>(expr, op, right, op.line, op.column);
    }
    
    return expr;
}

ExpressionPtr Parser::logicalAnd() {
    ExpressionPtr expr = equality();
    
    while (match(std::vector<TokenType>{TokenType::OP_AND})) {
        Token op = previous();
        ExpressionPtr right = equality();
        expr = std::make_shared<BinaryOp>(expr, op, right, op.line, op.column);
    }
    
    return expr;
}

ExpressionPtr Parser::equality() {
    ExpressionPtr expr = comparison();
    
    while (match(std::vector<TokenType>{TokenType::OP_EQ, TokenType::OP_NE})) {
        Token op = previous();
        ExpressionPtr right = comparison();
        expr = std::make_shared<BinaryOp>(expr, op, right, op.line, op.column);
    }
    
    return expr;
}

ExpressionPtr Parser::comparison() {
    ExpressionPtr expr = addition();
    
    while (match(std::vector<TokenType>{TokenType::OP_LT, TokenType::OP_LE,
                                        TokenType::OP_GT, TokenType::OP_GE})) {
        Token op = previous();
        ExpressionPtr right = addition();
        expr = std::make_shared<BinaryOp>(expr, op, right, op.line, op.column);
    }
    
    return expr;
}

ExpressionPtr Parser::addition() {
    ExpressionPtr expr = multiplication();
    
    while (match(std::vector<TokenType>{TokenType::OP_PLUS, TokenType::OP_MINUS})) {
        Token op = previous();
        ExpressionPtr right = multiplication();
        expr = std::make_shared<BinaryOp>(expr, op, right, op.line, op.column);
    }
    
    return expr;
}

ExpressionPtr Parser::multiplication() {
    ExpressionPtr expr = unary();
    
    while (match(std::vector<TokenType>{TokenType::OP_MULTIPLY, TokenType::OP_DIVIDE,
                                        TokenType::OP_MODULO})) {
        Token op = previous();
        ExpressionPtr right = unary();
        expr = std::make_shared<BinaryOp>(expr, op, right, op.line, op.column);
    }
    
    return expr;
}

ExpressionPtr Parser::unary() {
    if (match(std::vector<TokenType>{TokenType::OP_NOT, TokenType::OP_MINUS})) {
        Token op = previous();
        ExpressionPtr expr = unary();
        return std::make_shared<UnaryOp>(op, expr, op.line, op.column);
    }
    
    return primary();
}

ExpressionPtr Parser::primary() {
    if (check(TokenType::KW_TRUE) || check(TokenType::KW_FALSE)) {
        Token token = advance();
        return std::make_shared<Literal>(token, token.line, token.column);
    }
    
    if (check(TokenType::INT_LITERAL) || check(TokenType::FLOAT_LITERAL) ||
        check(TokenType::STRING_LITERAL)) {
        Token token = advance();
        return std::make_shared<Literal>(token, token.line, token.column);
    }
    
    if (check(TokenType::IDENTIFIER)) {
        Token token = advance();
        return std::make_shared<Variable>(token.lexeme, token.line, token.column);
    }
    
    if (match(std::vector<TokenType>{TokenType::LPAREN})) {
        ExpressionPtr expr = expression();
        consume(TokenType::RPAREN, "Expected ')' after expression");
        return expr;
    }
    
    Token token = peek();
    errorHandler->addError(ErrorType::SYNTAX_ERROR,
                         "Unexpected token: " + token.lexeme,
                         token.line, token.column,
                         "Expected an expression here");
    advance();
    return nullptr;
}

std::shared_ptr<Program> Parser::parse() {
    auto program = std::make_shared<Program>();
    
    while (!isAtEnd()) {
        StatementPtr stmt = declaration();
        if (stmt) {
            program->statements.push_back(stmt);
        }
        
        if (errorHandler->hasAnyErrors() && current > 0) {
            synchronize();
        }
    }
    
    return program;
}
