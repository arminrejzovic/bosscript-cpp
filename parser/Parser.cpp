//
// Created by Armin on 19.08.2023.
//

#include "Parser.h"

Program Parser::parseProgram(const std::string &src) {
    tokens = Lexer::tokenize(src, js);
    return Program(parseStatementList());
}

std::unique_ptr<Expression> Parser::parsePrimaryExpression() {
    switch (current().type) {
        case TokenType::Number:
            return parseNumericLiteral();
        case TokenType::DoubleQuote:
            return parseStringLiteral();
        case TokenType::OpenParen:
            return parseParenthesizedExpression();
        case TokenType::OpenBracket:
            return parseArrayLiteral();
        case TokenType::OpenBrace:
            return parseObjectLiteral();
        case TokenType::Tacno:
        case TokenType::Netacno:
            return parseBooleanLiteral();
        case TokenType::Nedefinisano:
            return parseNullLiteral();
        case TokenType::Javascript:
            return parseJavascriptSnippet();
        default:
            return parseIdentifier();
    }
}

std::unique_ptr<NumericLiteral> Parser::parseNumericLiteral() {
    return std::make_unique<NumericLiteral>(std::stod(consume().value));
}

std::unique_ptr<StringLiteral> Parser::parseStringLiteral() {
    consume(); //opening double quote
    Token strToken = expect(TokenType::String, "Unexpected token at [${getLineCol()}}]: String literal expected");
    expect(TokenType::DoubleQuote, "Expected closing double quote (\")");
    return std::make_unique<StringLiteral>(strToken.value);
}

std::unique_ptr<BooleanLiteral> Parser::parseBooleanLiteral() {
    return std::make_unique<BooleanLiteral>(consume().type == TokenType::Tacno);
}

std::unique_ptr<NullLiteral> Parser::parseNullLiteral() {
    consume();
    return std::make_unique<NullLiteral>();
}

std::unique_ptr<Identifier> Parser::parseIdentifier() {
    if(current().type == TokenType::This){
        consume(/* @ */);
        return std::make_unique<Identifier>("@");
    }
    std::string identifier = expect(TokenType::Identifier, "Identifier expected").value;
    return std::make_unique<Identifier>(identifier);
}

Token Parser::expect(TokenType expectedType, const std::string &errorMessage) {
    if(current().type != expectedType){
        throw std::runtime_error(errorMessage);
    }
    Token t = tokens.front();
    tokens.pop();
    return t;
}




