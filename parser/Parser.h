//
// Created by Armin on 19.08.2023.
//

#ifndef BOSSCRIPT_PARSER_H
#define BOSSCRIPT_PARSER_H


#include <stack>
#include "../lexer/Token.h"

class Parser {
private:
    bool js;
    std::stack<Token> tokens;

    Token current() {
        return tokens.top();
    }

    bool notEOF() {
        return current().type != TokenType::EndOfFile;
    }

    Token consume(){
        Token t = tokens.top();
        tokens.pop();
        return t;
    }
};


#endif //BOSSCRIPT_PARSER_H
