#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "error_handler.h"
#include <vector>
#include <memory>

// Forward declarations for AST nodes
struct ASTNode;
struct Program;
struct Statement;
struct Expression;

using ASTNodePtr = std::shared_ptr<ASTNode>;
using StatementPtr = std::shared_ptr<Statement>;
using ExpressionPtr = std::shared_ptr<Expression>;

// Base AST Node
struct ASTNode {
    int line;
    int column;
    virtual ~ASTNode() = default;
    ASTNode(int l = 0, int c = 0) : line(l), column(c) {}
};

// Expressions
struct Expression : public ASTNode {
    using ASTNode::ASTNode;
};

struct BinaryOp : public Expression {
    ExpressionPtr left;
    Token op;
    ExpressionPtr right;
    BinaryOp(ExpressionPtr l, Token o, ExpressionPtr r, int line, int col)
        : Expression(line, col), left(l), op(o), right(r) {}
};

struct UnaryOp : public Expression {
    Token op;
    ExpressionPtr operand;
    UnaryOp(Token o, ExpressionPtr operand, int line, int col)
        : Expression(line, col), op(o), operand(operand) {}
};

struct Literal : public Expression {
    Token value;
    Literal(Token v, int line, int col) : Expression(line, col), value(v) {}
};

struct Variable : public Expression {
    std::string name;
    Variable(const std::string& n, int line, int col)
        : Expression(line, col), name(n) {}
};

// Statements
struct Statement : public ASTNode {
    using ASTNode::ASTNode;
};

struct VarDeclaration : public Statement {
    std::string name;
    std::string type;
    ExpressionPtr initializer;
    VarDeclaration(const std::string& n, const std::string& t,
                   ExpressionPtr init, int line, int col)
        : Statement(line, col), name(n), type(t), initializer(init) {}
};

struct ExpressionStatement : public Statement {
    ExpressionPtr expr;
    ExpressionStatement(ExpressionPtr e, int line, int col)
        : Statement(line, col), expr(e) {}
};

struct IfStatement : public Statement {
    ExpressionPtr condition;
    StatementPtr thenBranch;
    StatementPtr elseBranch;
    IfStatement(ExpressionPtr cond, StatementPtr then, StatementPtr elseS,
                int line, int col)
        : Statement(line, col), condition(cond), thenBranch(then), elseBranch(elseS) {}
};

// FIX #7/#8: Block statement for scoped { } blocks
struct BlockStatement : public Statement {
    std::vector<StatementPtr> statements;
    BlockStatement(std::vector<StatementPtr> stmts, int line, int col)
        : Statement(line, col), statements(std::move(stmts)) {}
};

struct Program : public ASTNode {
    std::vector<StatementPtr> statements;
};

class Parser {
private:
    std::vector<Token> tokens;
    size_t current;
    std::shared_ptr<ErrorHandler> errorHandler;
    
    Token peek() const;
    Token previous() const;
    Token advance();
    bool check(TokenType type) const;
    bool match(const std::vector<TokenType>& types);
    bool isAtEnd() const;
    
    Token consume(TokenType type, const std::string& message);
    void synchronize();
    
    // Parsing methods
    StatementPtr statement();
    StatementPtr blockStatement();  // Scoped { } blocks
    StatementPtr declaration();
    StatementPtr varDeclaration();
    ExpressionPtr expression();
    ExpressionPtr assignment();
    ExpressionPtr logicalOr();
    ExpressionPtr logicalAnd();
    ExpressionPtr equality();
    ExpressionPtr comparison();
    ExpressionPtr addition();
    ExpressionPtr multiplication();
    ExpressionPtr unary();
    ExpressionPtr primary();
    
public:
    Parser(const std::vector<Token>& toks, std::shared_ptr<ErrorHandler> handler);
    
    std::shared_ptr<Program> parse();
};

#endif
