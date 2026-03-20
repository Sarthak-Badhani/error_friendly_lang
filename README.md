# Error-Friendly Programming Language Compiler

## Overview
A compiler for a beginner-friendly programming language designed to provide clear, helpful error messages that guide learners through their mistakes.

## Project Structure

```
error_friendly/
├── src/              # Implementation files
│   ├── main.cpp      # Compiler driver and entry point
│   ├── token.cpp     # Token definitions
│   ├── lexer.cpp     # Lexical analyzer
│   ├── parser.cpp    # Syntax analyzer
│   ├── error_handler.cpp    # Error reporting system
│   └── semantic_analyzer.cpp # Semantic analysis
├── include/          # Header files
│   ├── token.h
│   ├── lexer.h
│   ├── parser.h
│   ├── error_handler.h
│   ├── semantic_analyzer.h
│   └── compiler.h
├── tests/            # Test programs
└── docs/             # Documentation
```

## Compiler Architecture

The compiler follows a multi-pass compilation process:

1. **Lexical Analysis** - Tokenizes source code and detects lexical errors
2. **Syntax Analysis** - Parses tokens into an Abstract Syntax Tree (AST) with error recovery
3. **Semantic Analysis** - Validates type checking, variable declarations, and scope
4. **Error Reporting** - Generates user-friendly error messages with suggestions

## Language Features

### Data Types
- `int` - Integer values
- `float` - Floating-point numbers
- `bool` - Boolean values (true/false)
- `string` - Text strings

### Operators
- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
- Logical: `&&`, `||`, `!`
- Assignment: `=`

### Control Flow
- `if` statement
- `else` clause
- `while` loops (planned)
- `for` loops (planned)

### Variable Declaration
```
int x;
float pi = 3.14;
bool flag = true;
string message = "Hello";
```

## Building the Project

```bash
# Compile the entire project
# (Use the built-in C++ Build task in VS Code)
```

## Running the Compiler

```bash
# Compile a source file
compiler program.ef

# Interactive mode with sample code (no arguments)
compiler
```

## Error Messages

The compiler provides clear, educational error messages that include:
- Error type and location (line and column)
- Clear explanation of what went wrong
- Suggestions for fixing the error
- Context showing the problematic code

Example:
```
[Line 3, Column 5] UNDECLARED VARIABLE ERROR: Variable 'x' is not declared.
    Context: x = 42;
    Suggestion: Declare the variable before using it: int x;
```

## Test Cases

Sample test programs are provided in the `tests/` directory:
- `valid_program.ef` - Valid program without errors
- `lexical_errors.ef` - Demonstrates lexical error detection
- `syntax_errors.ef` - Demonstrates syntax error detection
- `semantic_errors.ef` - Demonstrates semantic error detection

## Future Enhancements

- [ ] Function definitions and calls
- [ ] Array support
- [ ] Additional built-in functions
- [ ] Code generation to intermediate representation
- [ ] Optimization passes
- [ ] REPL (Read-Eval-Print Loop) mode

## References

This compiler implementation is based on standard compiler design principles including:
- Lexical analysis using finite automata
- Recursive descent parsing
- AST-based semantic analysis
- Error recovery techniques (panic-mode recovery)

---
**Created as part of the Error-Friendly Programming Language Project**
