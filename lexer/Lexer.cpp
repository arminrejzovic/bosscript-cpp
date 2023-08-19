#include <regex>
#include <iostream>
#include "Lexer.h"

const std::map<std::string, TokenType> Lexer::keywords = {
    {"var", TokenType::Var},
    {"konst", TokenType::Konst},
    {"za", TokenType::Za},
    {"svako", TokenType::Svako},
    {"od", TokenType::Od},
    {"do", TokenType::Do},
    {"korak", TokenType::Korak},
    {"dok", TokenType::Dok},
    {"radi", TokenType::Radi},
    {"prekid", TokenType::Break},
    {"funkcija", TokenType::Funkcija},
    {"vrati", TokenType::Vrati},
    {"se", TokenType::Se},
    {"paket", TokenType::Paket},
    {"ako", TokenType::Ako},
    {"ili", TokenType::Ili},
    {"osim", TokenType::Osim},
    {"inace", TokenType::Inace},
    {"inače", TokenType::Inace},
    {"nedefinisano", TokenType::Nedefinisano},
    {"tacno", TokenType::Tacno},
    {"tačno", TokenType::Tacno},
    {"netacno", TokenType::Netacno},
    {"netačno", TokenType::Netacno},
    {"probaj", TokenType::Try},
    {"spasi", TokenType::Catch},
    {"svakako", TokenType::Finally},
    {"tip", TokenType::Tip},
    {"model", TokenType::Model},
    {"privatno", TokenType::Private},
    {"javno", TokenType::Public},
    {"konstruktor", TokenType::Constructor},
    {"konst", TokenType::Konst},
    {"konst", TokenType::Konst},
};

std::queue<Token> Lexer::tokenize(std::string &src, bool js) {
    std::queue<Token> tokens;
    size_t line = 1;
    size_t col = 1;
    size_t cursor = 0;

    while(cursor < src.size()){
        if(src[cursor] == '('){
            tokens.emplace(std::string(1, src[cursor++]), TokenType::OpenParen, line, col++);
        }
        else if(src[cursor] == ')'){
            tokens.emplace(std::string(1, src[cursor++]), TokenType::CloseParen, line, col++);
        }
        else if(src[cursor] == '['){
            tokens.emplace(std::string(1, src[cursor++]), TokenType::OpenBracket, line, col++);
        }
        else if(src[cursor] == ']'){
            tokens.emplace(std::string(1, src[cursor++]), TokenType::CloseBracket, line, col++);
        }
        else if(src[cursor] == '{'){
            tokens.emplace(std::string(1, src[cursor++]), TokenType::OpenBrace, line, col++);
        }
        else if(src[cursor] == '}'){
            tokens.emplace(std::string(1, src[cursor++]), TokenType::CloseBrace, line, col++);
        }
        else if(src[cursor] == '^'){
            tokens.emplace(std::string(1, src[cursor++]), TokenType::Exponent, line, col++);
        }
        else if(src[cursor] == '&'){
            if (src.size() > 1 && src[1] == '&'){
                tokens.emplace("&&", TokenType::LogicalAnd, line, col);
                cursor += 2;
                col += 2;
            }
            else {
                throw std::runtime_error(
                        "Unexpected token found at " + std::string(1, line) + std::string(1, col) + ": " + src[cursor]
                );
            }
        }
        else if(src[cursor] == '|'){
            if (src.size() > 1 && src[1] == '|'){
                tokens.emplace("||", TokenType::LogicalOr, line, col);
                cursor += 2;
                col += 2;
            }
            else {
                throw std::runtime_error(
                        "Unexpected token found at " + std::string(1, line) + std::string(1, col) + ": " + src[cursor]
                );
            }
        }
        else if(src[cursor] == '+' || src[cursor] == '-' || src[cursor] == '*' || src[cursor] == '/' || src[cursor] == '%'){
            if(src.size() > 1 && src[cursor + 1] == '='){
                std::string p1 = std::string(1, src[cursor]);
                std::string p2 = std::string(1, src[cursor+1]);
                tokens.emplace(p1 + p2, TokenType::ComplexAssign, line, col);
                cursor += 2;
                col += 2;
            }
            else if(src.size() > 1 && src[cursor] == '+' && src[cursor + 1] == '+'){
                tokens.emplace("++", TokenType::UnaryIncrement, line, col);
                cursor += 2;
                col += 2;
            }
            else if(src.size() > 1 && src[cursor] == '-' && src[cursor + 1] == '-'){
                tokens.emplace("--", TokenType::UnaryDecrement, line, col);
                cursor += 2;
                col += 2;
            }
            else{
                tokens.emplace(std::string(1, src[cursor++]), TokenType::BinaryOperator, line, col++);
            }
        }

        else if(src[cursor] == '='){
            if(src.size() > 1 && src[cursor + 1] == '='){
                tokens.emplace("==", TokenType::EqualityOperator, line, col);
                cursor += 2;
                col += 2;
            }
            else if(src.size() > 1 && src[cursor + 1] == '>'){
                tokens.emplace("=>", TokenType::Arrow, line, col);
                cursor += 2;
                col += 2;
            }
            else{
                tokens.emplace(std::string(1, src[cursor++]), TokenType::SimpleAssign, line, col++);
            }
        }

        else if(src[cursor] == '!'){
            if(src.size() > 1 && src[cursor + 1] == '='){
                tokens.emplace("!=", TokenType::EqualityOperator, line, col);
                cursor += 2;
                col += 2;
            }
            else{
                tokens.emplace(std::string(1, src[cursor++]), TokenType::LogicalNot, line, col++);
            }
        }

        else if(src[cursor] == ':'){
            tokens.emplace(std::string(1, src[cursor++]), TokenType::Colon, line, col++);
        }
        else if(src[cursor] == ';'){
            tokens.emplace(std::string(1, src[cursor++]), TokenType::Semicolon, line, col++);
        }
        else if(src[cursor] == ','){
            tokens.emplace(std::string(1, src[cursor++]), TokenType::Comma, line, col++);
        }
        else if(src[cursor] == '.'){
            tokens.emplace(std::string(1, src[cursor++]), TokenType::Dot, line, col++);
        }
        else if(src[cursor] == '@'){
            tokens.emplace(std::string(1, src[cursor++]), TokenType::This, line, col++);
        }

        else if(src[cursor] == '<' || src[cursor] == '>'){
            if(src.size() > 1 && src[1] == '='){
                std::string p1 = std::string(1, src[cursor]);
                std::string p2 = std::string(1, src[cursor+1]);
                tokens.emplace(p1 + p2, TokenType::RelationalOperator, line, col);
                cursor += 2;
                col += 2;
            }
            else{
                tokens.emplace(std::string(1, src[cursor++]), TokenType::RelationalOperator, line, col++);
            }
        }

        else if(src[cursor] == '\n'){
            line++;
            col = 1;
            cursor++;
        }

        else if(src[cursor] == ' ' or src[cursor] == '\r' or src[cursor] == '\t'){
            cursor++;
            col += 1;
        }

        else{
            // Multi-character tokens
            if(src[cursor] == '"'){
                //String literals
                // Add the opening quotation mark
                tokens.emplace(std::string(1, src[cursor++]), TokenType::DoubleQuote, line, col++);
                std::string str;
                while (!src.empty() && src[cursor] != '"'){
                    if(src[cursor] == '\\' && src.size() > 1){
                        switch (src[cursor + 1]) {
                            case 'n':
                                str += "\n";
                                break;
                            case 't':
                                str += "\t";
                                break;
                            case 'r':
                                str += "\r";
                                break;
                            case '\\':
                                str += "\\";
                                break;
                            case '"':
                                str += '"';
                                break;
                        }

                        cursor += 2;
                    }
                    else{
                        str += src[cursor++];
                    }
                }
                // Add the string value
                tokens.emplace(str, TokenType::String, line, col);
                col += str.size();

                // If exists, add closing quotation mark
                if(!src.empty() && src[cursor] == '"'){
                    tokens.emplace(std::string(1, src[cursor++]), TokenType::DoubleQuote, line, col++);
                }
            }

                // Numbers
            else if(isdigit(src[cursor])){
                std::string number;
                while (!src.empty() && isdigit(src[cursor]) || src[cursor] == '_' || src[cursor] == '.'){
                    number += src[cursor++];
                }
                std::regex validNumberPattern(R"regex(^-?(0|[1-9](_?[0-9])*)(\.[0-9](_?[0-9])*)?([eE][-+]?[0-9]+)?)regex");

                if(std::regex_match(number, validNumberPattern)){
                    number = std::regex_replace(number, std::regex("_"), "");
                    tokens.emplace(number, TokenType::Number, line, col);
                    col += number.size();
                }
                else{
                    throw std::runtime_error("Unexpected token found at " + std::string(1, line) + std::string(1, col) + ": " + src[cursor]);
                }
            }

            // Identifiers
            else if(isalpha(src[cursor]) or src[cursor] == '$' or src[cursor] == '_'){
                std::string identifier = std::string(1, src[cursor++]);
                while (!src.empty() && isalpha(src[cursor]) or isdigit(src[cursor]) or src[cursor] == '$' or src[cursor] == '_'){
                    identifier += std::string(1, src[cursor++]);
                }
                // Check for reserved Keywords
                auto reservedPosition = keywords.find(identifier);
                if (reservedPosition != keywords.end()){
                    tokens.emplace(identifier, reservedPosition->second, line, col);
                }
                else {
                    tokens.emplace(identifier, TokenType::Identifier, line, col);
                }
                col += identifier.size();
            }

            else if(src[cursor] == '`'){
                if(!js){
                    throw std::runtime_error("Javascript snippets are not allowed here.");
                }
                std::string snippet;
                cursor++;
                while (!src.empty() && src[cursor] != '`'){
                    if(src[cursor] == '\n'){
                        line++;
                        col = 1;
                    }
                    snippet += src[cursor++];
                    col++;
                }
                if(src[cursor] != '`'){
                    throw std::runtime_error("Missing closing backtick");
                }
                cursor++;
                tokens.emplace(snippet, TokenType::Javascript, line, col);
            }

            else {
                // Something unexpected
                throw std::runtime_error("Unexpected token found at[$line:$col]: '${src[cursor]}'");
            }
        }
    }
    tokens.emplace("EOF", TokenType::EndOfFile, line, col);
    return tokens;
}

Lexer::Lexer() = default;

