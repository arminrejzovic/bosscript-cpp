//
// Created by Armin on 15.08.2023.
//

#ifndef BOSSCRIPT_TOKEN_H
#define BOSSCRIPT_TOKEN_H
#endif //BOSSCRIPT_TOKEN_H


#include <string>
#include "TokenType.h"


class Token {
public:
    std::string value;
    TokenType type;
    size_t line;
    size_t col;

    Token(std::string value, TokenType type, size_t line, size_t col);

    virtual ~Token();

    friend std::ostream &operator<<(std::ostream &os, const Token &token);

    std::string getLineCol();
};
