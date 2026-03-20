#include "../include/lexer.h"
#include <unordered_map>
#include <cctype>

std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"int", TokenType::KW_INT},
    {"float", TokenType::KW_FLOAT},
    {"bool", TokenType::KW_BOOL},
    {"string", TokenType::KW_STRING},
    {"if", TokenType::KW_IF},
    {"else", TokenType::KW_ELSE},
    {"while", TokenType::KW_WHILE},
    {"for", TokenType::KW_FOR},
    {"function", TokenType::KW_FUNCTION},
    {"return", TokenType::KW_RETURN},
    {"true", TokenType::KW_TRUE},
    {"false", TokenType::KW_FALSE}
};

Lexer::Lexer(const std::string& src, std::shared_ptr<ErrorHandler> handler)
    : source(src), current(0), start(0), line(1), column(1), errorHandler(handler) {}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::peekNext() const {
    if (current + 1 >= source.size()) return '\0';
    return source[current + 1];
}

char Lexer::advance() {
    char c = source[current++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

bool Lexer::isAtEnd() const {
    return current >= source.size();
}

bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;
    advance();
    return true;
}

Token Lexer::makeToken(TokenType type) {
    std::string lexeme = source.substr(start, current - start);
    return Token(type, lexeme, line, column - static_cast<int>(lexeme.size()));
}

Token Lexer::makeToken(TokenType type, const std::string& value) {
    std::string lexeme = source.substr(start, current - start);
    return Token(type, lexeme, line, column - static_cast<int>(lexeme.size()), value);
}

Token Lexer::errorToken(const std::string& message) {
    int startCol = column - static_cast<int>(source.substr(start, current - start).size());
    Token token(TokenType::ERROR_TOKEN, "", line, startCol);
    errorHandler->addError(ErrorType::LEXICAL_ERROR, message, line, startCol);
    return token;
}

Token Lexer::scanString() {
    std::string value;
    while (!isAtEnd() && peek() != '"') {
        if (peek() == '\\') {
            advance();
            if (!isAtEnd()) {
                char escape = advance();
                switch (escape) {
                    case 'n': value += '\n'; break;
                    case 't': value += '\t'; break;
                    case '\\': value += '\\'; break;
                    case '"': value += '"'; break;
                    default: value += escape;
                }
            }
        } else {
            value += advance();
        }
    }
    
    if (isAtEnd()) {
        return errorToken("Unterminated string literal. Did you forget to close the string with \"?");
    }
    
    advance();  // closing "
    return makeToken(TokenType::STRING_LITERAL, value);
}

Token Lexer::scanNumber() {
    while (isDigit(peek())) advance();
    
    // Check for decimal point
    if (peek() == '.' && isDigit(peekNext())) {
        advance();  // consume .
        while (isDigit(peek())) advance();
        std::string value = source.substr(start, current - start);
        return makeToken(TokenType::FLOAT_LITERAL, value);
    }
    
    std::string value = source.substr(start, current - start);
    return makeToken(TokenType::INT_LITERAL, value);
}

Token Lexer::scanIdentifier() {
    while (isAlphaNumeric(peek())) advance();
    
    std::string text = source.substr(start, current - start);
    TokenType type = TokenType::IDENTIFIER;
    
    if (keywords.find(text) != keywords.end()) {
        type = keywords[text];
    }
    
    return makeToken(type);
}

Token Lexer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\r' || c == '\t') {
            advance();
        } else if (c == '/' && peekNext() == '/') {
            // Single-line comment
            while (!isAtEnd() && peek() != '\n') advance();
        } else if (c == '/' && peekNext() == '*') {
            // Multi-line comment
            advance();  // /
            advance();  // *
            while (!isAtEnd()) {
                if (peek() == '*' && peekNext() == '/') {
                    advance();  // *
                    advance();  // /
                    break;
                }
                advance();
            }
        } else {
            return makeToken(TokenType::ERROR_TOKEN);  // Placeholder
        }
    }
    return makeToken(TokenType::ERROR_TOKEN);  // Placeholder
}

Token Lexer::nextToken() {
    while (!isAtEnd()) {
        start = current;
        char c = advance();
        
        // Whitespace
        if (c == ' ' || c == '\r' || c == '\t') continue;
        if (c == '\n') return makeToken(TokenType::NEWLINE);
        
        // Comments
        if (c == '/' && peek() == '/') {
            while (!isAtEnd() && peek() != '\n') advance();
            continue;
        }
        if (c == '/' && peek() == '*') {
            while (!isAtEnd() && !(peek() == '*' && peekNext() == '/')) {
                advance();
            }
            if (!isAtEnd()) { advance(); advance(); }
            continue;
        }
        
        // Strings
        if (c == '"') return scanString();
        
        // Numbers
        if (isDigit(c)) {
            current--;
            return scanNumber();
        }
        
        // Identifiers and keywords
        if (isAlpha(c)) {
            current--;
            return scanIdentifier();
        }
        
        // Operators and delimiters
        switch (c) {
            case '+': return makeToken(TokenType::OP_PLUS);
            case '-': return makeToken(TokenType::OP_MINUS);
            case '*': return makeToken(TokenType::OP_MULTIPLY);
            case '/': return makeToken(TokenType::OP_DIVIDE);
            case '%': return makeToken(TokenType::OP_MODULO);
            case '=': return makeToken(match('=') ? TokenType::OP_EQ : TokenType::OP_ASSIGN);
            case '!': return makeToken(match('=') ? TokenType::OP_NE : TokenType::OP_NOT);
            case '<': return makeToken(match('=') ? TokenType::OP_LE : TokenType::OP_LT);
            case '>': return makeToken(match('=') ? TokenType::OP_GE : TokenType::OP_GT);
            case '&': 
                if (match('&')) return makeToken(TokenType::OP_AND);
                return errorToken("Unexpected character '&'. Did you mean '&&'?");
            case '|':
                if (match('|')) return makeToken(TokenType::OP_OR);
                return errorToken("Unexpected character '|'. Did you mean '||'?");
            case '(': return makeToken(TokenType::LPAREN);
            case ')': return makeToken(TokenType::RPAREN);
            case '{': return makeToken(TokenType::LBRACE);
            case '}': return makeToken(TokenType::RBRACE);
            case '[': return makeToken(TokenType::LBRACKET);
            case ']': return makeToken(TokenType::RBRACKET);
            case ';': return makeToken(TokenType::SEMICOLON);
            case ',': return makeToken(TokenType::COMMA);
            case ':': return makeToken(TokenType::COLON);
            case '.': return makeToken(TokenType::DOT);
            default: {
                std::string msg = "Unexpected character: '";
                msg += c;
                msg += "'. This character is not recognized in the language.";
                return errorToken(msg);
            }
        }
    }
    
    return Token(TokenType::END_OF_FILE, "", line, column);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> result;
    Token token = nextToken();
    
    while (token.type != TokenType::END_OF_FILE) {
        if (token.type != TokenType::NEWLINE && token.type != TokenType::ERROR_TOKEN) {
            result.push_back(token);
        }
        token = nextToken();
    }
    
    result.push_back(token);  // Add EOF token
    return result;
}
