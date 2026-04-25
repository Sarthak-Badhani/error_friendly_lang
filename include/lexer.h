#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "error_handler.h"
#include <vector>
#include <string>
#include <memory>

class Lexer {
private:
    std::string source;
    size_t current;
    size_t start;
    int line;
    int column;
    int startColumn;  // Column at the start of current token
    std::shared_ptr<ErrorHandler> errorHandler;
    
    static std::unordered_map<std::string, TokenType> keywords;
    
    char peek() const;
    char peekNext() const;
    char advance();
    bool isAtEnd() const;
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
    bool match(char expected);
    
    Token makeToken(TokenType type);
    Token makeToken(TokenType type, const std::string& value);
    Token errorToken(const std::string& message);
    
    Token scanString();
    Token scanNumber();
    Token scanIdentifier();
    Token skipWhitespace();
    
public:
    Lexer(const std::string& src, std::shared_ptr<ErrorHandler> handler);
    
    Token nextToken();
    std::vector<Token> tokenize();
};

#endif
