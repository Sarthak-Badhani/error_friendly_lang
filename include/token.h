#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <unordered_map>

enum class TokenType {
    // Keywords
    KW_INT,
    KW_FLOAT,
    KW_BOOL,
    KW_STRING,
    KW_IF,
    KW_ELSE,
    KW_WHILE,
    KW_FOR,
    KW_FUNCTION,
    KW_RETURN,
    KW_TRUE,
    KW_FALSE,
    
    // Operators
    OP_PLUS,      // +
    OP_MINUS,     // -
    OP_MULTIPLY,  // *
    OP_DIVIDE,    // /
    OP_MODULO,    // %
    OP_ASSIGN,    // =
    OP_EQ,        // ==
    OP_NE,        // !=
    OP_LT,        // <
    OP_LE,        // <=
    OP_GT,        // >
    OP_GE,        // >=
    OP_AND,       // &&
    OP_OR,        // ||
    OP_NOT,       // !
    
    // Delimiters
    LPAREN,       // (
    RPAREN,       // )
    LBRACE,       // {
    RBRACE,       // }
    LBRACKET,     // [
    RBRACKET,     // ]
    SEMICOLON,    // ;
    COMMA,        // ,
    COLON,        // :
    DOT,          // .
    
    // Literals
    IDENTIFIER,
    INT_LITERAL,
    FLOAT_LITERAL,
    STRING_LITERAL,
    
    // Special
    END_OF_FILE,
    NEWLINE,
    ERROR_TOKEN
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
    std::string value;  // For literals
    
    Token(TokenType t = TokenType::ERROR_TOKEN, 
          const std::string& lex = "", 
          int l = 0, 
          int c = 0, 
          const std::string& val = "")
        : type(t), lexeme(lex), line(l), column(c), value(val) {}
};

// Utility function to convert TokenType to string
std::string tokenTypeToString(TokenType type);

#endif
