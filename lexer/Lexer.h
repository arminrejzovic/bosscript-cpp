#ifndef BOSSCRIPT_LEXER_H
#define BOSSCRIPT_LEXER_H

#include <map>
#include <vector>
#include "Token.h"
#include <stdexcept>

class Lexer {
public:
    static const std::map<std::string, TokenType> keywords;
    std::vector<Token> tokenize(std::string &src, bool js);

    Lexer();
};


#endif //BOSSCRIPT_LEXER_H
