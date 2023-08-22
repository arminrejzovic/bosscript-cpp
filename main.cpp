#include <iostream>
#include <fstream>
#include <stdexcept>
#include "lexer/Lexer.h"
#include "parser/Parser.h"

#include <chrono>
using namespace std::chrono;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    std::ifstream file(filename);

    if(file){
        std::string src((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());

        auto start = high_resolution_clock::now();
        Parser p(false);
        auto program = p.parseProgram(src);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        std::cout << "Program parsed in " << duration.count() << "ms" << std::endl;
    }
    else {
        std::cout << "Failed to open file " << filename << std::endl;
    }
    return 0;
}
