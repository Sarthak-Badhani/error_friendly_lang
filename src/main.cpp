#include "compiler.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "==============================================================" << std::endl;
    std::cout << "         Error-Friendly Programming Language Compiler         " << std::endl;
    std::cout << "                  A Beginner-Focused Compiler                 " << std::endl;
    std::cout << "==============================================================" << std::endl << std::endl;
    
    Compiler compiler;
    bool success = false;
    
    if (argc < 2) {
        // Interactive mode
        std::cout << "Usage: compiler <filename>" << std::endl;
        std::cout << "Example: compiler program.ef" << std::endl << std::endl;
        
        // Test with sample code
        std::string sampleCode = R"(
int x;
x = 42;
int y = x + 8;
)";
        
        std::cout << "Compiling sample code..." << std::endl << std::endl;
        success = compiler.compile(sampleCode);
    } else {
        success = compiler.compileFromFile(argv[1]);
    }
    
    if (!success && !compiler.hasErrors()) {
        std::cout << "=== COMPILATION FAILED ===" << std::endl;
        std::cout << "Could not compile input file. Check the file path and try again." << std::endl;
    } else {
        compiler.displayResults();
    }
    
    return success ? 0 : 1;
}
