# Error-Friendly Programming Language

## Overview
The **Error-Friendly Programming Language** is a beginner-focused full-stack ecosystem consisting of a custom compiler and a web-based IDE. It is specifically designed to provide clear, helpful, and educational error messages that guide learners through their mistakes rather than overwhelming them with cryptic technical jargon.

## What It Is
Instead of typical command-line compilation errors, this project provides a modern web interface where users can write code and instantly see compilation results in a split-pane terminal. The core compiler is written in C++ and handles custom language syntax, complete with strict type checking, block-scoped variable resolution, and panic-mode error recovery. 

*(For a deep dive into the compiler's architecture, stages, and implementation details, please see the `IMPLEMENTATION_SUMMARY.md` file.)*

## Tech Stack

**Frontend**
- **React (Vite)**: High-performance modern web framework.
- **Monaco Editor**: Provides the core code editing experience, syntax highlighting, and line numbers.
- **Vanilla CSS**: Used for all custom retro/dark-mode styling and layout.

**Backend**
- **Node.js & Express**: Lightweight REST API to handle incoming compilation requests.
- **Child Process Orchestration**: Securely invokes the C++ compiler executable on temporary source files.

**Compiler Core**
- **C++17**: High-performance systems language used to build the lexer, parser, and semantic analyzer from scratch.
- **MinGW G++**: Build toolchain used to statically link and compile the executable.

## Language Basics

The custom `.ef` language includes basic types, block scoping, and control flow:
```cpp
// Variable Declarations
int age = 20;
float pi = 3.14;
bool isDone = false;
string message = "Hello World";

// Control Flow
if (age >= 18) {
    string status = "Adult";
}
```

## Running the Project

To run the entire ecosystem locally, you will need to start both the backend server and the frontend client.

### 1. Build the C++ Compiler
*(Ensure you have MinGW installed on your system)*
```bash
# In the root directory, compile the binary:
mingw32-make
# OR manually compile:
g++ -std=c++17 -Wall -Wextra -static-libgcc -static-libstdc++ -Iinclude src/*.cpp -o bin/compiler.exe
```

### 2. Start the Backend Server
The Node.js server acts as the bridge between the React app and the C++ binary.
```bash
cd backend
npm install
node server.js
# The backend will run on http://localhost:5000
```

### 3. Start the Frontend App
The Vite React app serves the web interface.
```bash
cd frontend
npm install
npm run dev
# The frontend will run on http://localhost:5173
```

Open `http://localhost:5173` in your browser to start writing code!

## Educational Error Messages

The primary feature of this language is its error reporting. When a user makes a mistake, the compiler attempts to diagnose *why* it happened and offers a human-readable suggestion.

Example of a caught Type Error:
```
[Line 1, Column 5] TYPE ERROR: Cannot assign 'string' value to variable 'x' of type 'int'.
    Suggestion: Make sure the value matches the declared type. Expected 'int' but got 'string'.
```

Example of a caught Syntax Error:
```
[Line 3, Column 1] SYNTAX ERROR: Expected ';' after variable declaration.
    Suggestion: Add a ';' at the end: int x = ...;
```

---
*Created as part of the Error-Friendly Programming Language Project*
