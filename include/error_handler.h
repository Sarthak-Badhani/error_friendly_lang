#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <string>
#include <vector>
#include <memory>

enum class ErrorType {
    LEXICAL_ERROR,
    SYNTAX_ERROR,
    SEMANTIC_ERROR,
    TYPE_ERROR,
    UNDECLARED_VARIABLE,
    REDECLARED_VARIABLE,
    SCOPE_ERROR,
    UNDEFINED_FUNCTION,
    INVALID_OPERATOR,
    UNKNOWN_ERROR
};

struct CompilerError {
    ErrorType type;
    std::string message;
    int line;
    int column;
    std::string suggestion;
    std::string context;  // The problematic line of code
    
    CompilerError(ErrorType t, const std::string& msg, int l, int c,
                  const std::string& sugg = "", const std::string& ctx = "")
        : type(t), message(msg), line(l), column(c), suggestion(sugg), context(ctx) {}
};

class ErrorHandler {
private:
    std::vector<CompilerError> errors;
    bool hasErrors;
    
    std::string errorTypeToString(ErrorType type) const;
    std::string formatErrorMessage(const CompilerError& error) const;
    
public:
    ErrorHandler();
    
    void addError(ErrorType type, const std::string& message, int line, int column,
                  const std::string& suggestion = "", const std::string& context = "");
    
    void reportError(const CompilerError& error);
    void reportAllErrors();
    
    bool hasAnyErrors() const { return hasErrors; }
    int getErrorCount() const { return errors.size(); }
    const std::vector<CompilerError>& getErrors() const { return errors; }
    
    void clearErrors();
};

#endif
