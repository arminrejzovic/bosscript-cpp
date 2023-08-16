//
// Created by Armin on 15.08.2023.
//

#include "Token.h"
#include <ostream>

Token::Token(std::string value, TokenType type, size_t line, size_t col) : value(std::move(value)), type(type), line(line), col(col) {}

Token::~Token() = default;


std::string Token::getLineCol() {
    return std::to_string(line) + ":" + std::to_string(col);
}

std::ostream &operator<<(std::ostream &os, const Token &token) {
    os << "TOKEN [" << "value: " << token.value << " type: " << token.type << " line: " << token.line << " col: "
       << token.col << "]";
    return os;
}
