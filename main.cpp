#include <iostream>
#include <fstream>
#include <stdexcept>
#include "lexer/Lexer.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1; // Return an error code
    }

    std::string filename = argv[1];

    std::ifstream file(filename);

    if(file){
        std::cout << "[INFO] Successfully opened file " << filename << std::endl;

        std::string src((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

        std::cout << "[INFO]\n" << src << std::endl;
        std::cout << "******************************" << std::endl;

        Lexer lexer;
        auto tokens = lexer.tokenize(src, false);
        for (const auto &item: tokens){
            std::cout << item << std::endl;
        }
    }
    else {
        std::cout << "Failed to open file " << filename << std::endl;
    }
    return 0;
}
