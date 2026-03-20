#include "../include/token.h"

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::KW_INT: return "INT";
        case TokenType::KW_FLOAT: return "FLOAT";
        case TokenType::KW_BOOL: return "BOOL";
        case TokenType::KW_STRING: return "STRING";
        case TokenType::KW_IF: return "IF";
        case TokenType::KW_ELSE: return "ELSE";
        case TokenType::KW_WHILE: return "WHILE";
        case TokenType::KW_FOR: return "FOR";
        case TokenType::KW_FUNCTION: return "FUNCTION";
        case TokenType::KW_RETURN: return "RETURN";
        case TokenType::KW_TRUE: return "TRUE";
        case TokenType::KW_FALSE: return "FALSE";
        case TokenType::OP_PLUS: return "PLUS";
        case TokenType::OP_MINUS: return "MINUS";
        case TokenType::OP_MULTIPLY: return "MULTIPLY";
        case TokenType::OP_DIVIDE: return "DIVIDE";
        case TokenType::OP_MODULO: return "MODULO";
        case TokenType::OP_ASSIGN: return "ASSIGN";
        case TokenType::OP_EQ: return "EQ";
        case TokenType::OP_NE: return "NE";
        case TokenType::OP_LT: return "LT";
        case TokenType::OP_LE: return "LE";
        case TokenType::OP_GT: return "GT";
        case TokenType::OP_GE: return "GE";
        case TokenType::OP_AND: return "AND";
        case TokenType::OP_OR: return "OR";
        case TokenType::OP_NOT: return "NOT";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::LBRACKET: return "LBRACKET";
        case TokenType::RBRACKET: return "RBRACKET";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::COLON: return "COLON";
        case TokenType::DOT: return "DOT";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::INT_LITERAL: return "INT_LITERAL";
        case TokenType::FLOAT_LITERAL: return "FLOAT_LITERAL";
        case TokenType::STRING_LITERAL: return "STRING_LITERAL";
        case TokenType::END_OF_FILE: return "EOF";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::ERROR_TOKEN: return "ERROR";
        default: return "UNKNOWN";
    }
}
