#include "../include/error_handler.h"
#include <iostream>
#include <iomanip>

std::string ErrorHandler::errorTypeToString(ErrorType type) const {
    switch (type) {
        case ErrorType::LEXICAL_ERROR: return "LEXICAL ERROR";
        case ErrorType::SYNTAX_ERROR: return "SYNTAX ERROR";
        case ErrorType::SEMANTIC_ERROR: return "SEMANTIC ERROR";
        case ErrorType::TYPE_ERROR: return "TYPE ERROR";
        case ErrorType::UNDECLARED_VARIABLE: return "UNDECLARED VARIABLE ERROR";
        case ErrorType::REDECLARED_VARIABLE: return "REDECLARED VARIABLE ERROR";
        case ErrorType::SCOPE_ERROR: return "SCOPE ERROR";
        case ErrorType::UNDEFINED_FUNCTION: return "UNDEFINED FUNCTION ERROR";
        case ErrorType::INVALID_OPERATOR: return "INVALID OPERATOR ERROR";
        default: return "UNKNOWN ERROR";
    }
}

std::string ErrorHandler::formatErrorMessage(const CompilerError& error) const {
    std::string formatted;
    formatted += "[Line " + std::to_string(error.line) + ", Column " + 
                 std::to_string(error.column) + "] ";
    formatted += errorTypeToString(error.type) + ": " + error.message;
    
    if (!error.context.empty()) {
        formatted += "\n    Context: " + error.context;
    }
    
    if (!error.suggestion.empty()) {
        formatted += "\n    Suggestion: " + error.suggestion;
    }
    
    return formatted;
}

ErrorHandler::ErrorHandler() : hasErrors(false) {}

void ErrorHandler::addError(ErrorType type, const std::string& message, int line, int column,
                           const std::string& suggestion, const std::string& context) {
    CompilerError error(type, message, line, column, suggestion, context);
    errors.push_back(error);
    hasErrors = true;
}

void ErrorHandler::reportError(const CompilerError& error) {
    std::cerr << formatErrorMessage(error) << std::endl;
}

void ErrorHandler::reportAllErrors() {
    if (errors.empty()) {
        std::cout << "✓ Compilation successful! No errors found." << std::endl;
        return;
    }
    
    std::cout << "=== COMPILATION ERRORS ===" << std::endl;
    std::cout << "Total errors: " << errors.size() << std::endl << std::endl;
    
    for (const auto& error : errors) {
        reportError(error);
        std::cout << std::endl;
    }
}

void ErrorHandler::clearErrors() {
    errors.clear();
    hasErrors = false;
}
