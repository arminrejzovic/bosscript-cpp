#include <regex>
#include <iostream>
#include "Lexer.h"

const std::map<std::string, TokenType> Lexer::keywords = {
    {"var", Var},
    {"konst", Konst},
    {"za", Za},
    {"svako", Svako},
    {"od", Od},
    {"do", Do},
    {"korak", Korak},
    {"dok", Dok},
    {"radi", Radi},
    {"prekid", Break},
    {"funkcija", Funkcija},
    {"vrati", Vrati},
    {"se", Se},
    {"paket", Paket},
    {"ako", Ako},
    {"ili", Ili},
    {"osim", Osim},
    {"inace", Inace},
    {"inače", Inace},
    {"nedefinisano", Nedefinisano},
    {"tacno", Tacno},
    {"tačno", Tacno},
    {"netacno", Netacno},
    {"netačno", Netacno},
    {"probaj", Try},
    {"spasi", Catch},
    {"svakako", Finally},
    {"tip", Tip},
    {"model", Model},
    {"privatno", Private},
    {"javno", Public},
    {"konstruktor", Constructor},
    {"konst", Konst},
    {"konst", Konst},
};

std::vector<Token> Lexer::tokenize(std::string &src, bool js) {
    std::vector<Token> tokens;
    size_t line = 1;
    size_t col = 1;
    size_t cursor = 0;

    while(cursor < src.size()){
        if(src[cursor] == '('){
            tokens.emplace_back(std::string(1, src[cursor++]), OpenParen, line, col++);
        }
        else if(src[cursor] == ')'){
            tokens.emplace_back(std::string(1, src[cursor++]), CloseParen, line, col++);
        }
        else if(src[cursor] == '['){
            tokens.emplace_back(std::string(1, src[cursor++]), OpenBracket, line, col++);
        }
        else if(src[cursor] == ']'){
            tokens.emplace_back(std::string(1, src[cursor++]), CloseBracket, line, col++);
        }
        else if(src[cursor] == '{'){
            tokens.emplace_back(std::string(1, src[cursor++]), OpenBrace, line, col++);
        }
        else if(src[cursor] == '}'){
            tokens.emplace_back(std::string(1, src[cursor++]), CloseBrace, line, col++);
        }
        else if(src[cursor] == '^'){
            tokens.emplace_back(std::string(1, src[cursor++]), Exponent, line, col++);
        }
        else if(src[cursor] == '&'){
            if (src.size() > 1 && src[1] == '&'){
                tokens.emplace_back(std::string(1, src[cursor] + src[cursor+1]), LogicalAnd, line, col);
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
                tokens.emplace_back(std::string(1, src[cursor] + src[cursor+1]), LogicalOr, line, col);
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
                tokens.emplace_back(std::string(1, src[cursor] + src[cursor+1]), ComplexAssign, line, col);
                cursor += 2;
                col += 2;
            }
            else if(src.size() > 1 && src[cursor] == '+' && src[cursor + 1] == '+'){
                tokens.emplace_back(std::string(1, src[cursor] + src[cursor+1]), UnaryIncrement, line, col);
                cursor += 2;
                col += 2;
            }
            else if(src.size() > 1 && src[cursor] == '-' && src[cursor + 1] == '-'){
                tokens.emplace_back(std::string(1, src[cursor] + src[cursor+1]), UnaryDecrement, line, col);
                cursor += 2;
                col += 2;
            }
            else{
                tokens.emplace_back(std::string(1, src[cursor++]), BinaryOperator, line, col++);
            }
        }

        else if(src[cursor] == '='){
            if(src.size() > 1 && src[cursor + 1] == '='){
                tokens.emplace_back(std::string(1, src[cursor] + src[cursor+1]), EqualityOperator, line, col);
                cursor += 2;
                col += 2;
            }
            else if(src.size() > 1 && src[cursor + 1] == '>'){
                tokens.emplace_back(std::string(1, src[cursor] + src[cursor+1]), Arrow, line, col);
                cursor += 2;
                col += 2;
            }
            else{
                tokens.emplace_back(std::string(1, src[cursor++]), SimpleAssign, line, col++);
            }
        }

        else if(src[cursor] == '!'){
            if(src.size() > 1 && src[cursor + 1] == '='){
                tokens.emplace_back(std::string(1, src[cursor] + src[cursor+1]), EqualityOperator, line, col);
                cursor += 2;
                col += 2;
            }
            else{
                tokens.emplace_back(std::string(1, src[cursor++]), LogicalNot, line, col++);
            }
        }

        else if(src[cursor] == ':'){
            tokens.emplace_back(std::string(1, src[cursor++]), Colon, line, col++);
        }
        else if(src[cursor] == ';'){
            tokens.emplace_back(std::string(1, src[cursor++]), Semicolon, line, col++);
        }
        else if(src[cursor] == ','){
            tokens.emplace_back(std::string(1, src[cursor++]), Comma, line, col++);
        }
        else if(src[cursor] == '.'){
            tokens.emplace_back(std::string(1, src[cursor++]), Dot, line, col++);
        }
        else if(src[cursor] == '@'){
            tokens.emplace_back(std::string(1, src[cursor++]), This, line, col++);
        }

        else if(src[cursor] == '<' || src[cursor] == '>'){
            if(src.size() > 1 && src[1] == '='){
                tokens.emplace_back(std::string(1, src[cursor] + src[cursor+1]), RelationalOperator, line, col);
                cursor += 2;
                col += 2;
            }
            else{
                tokens.emplace_back(std::string(1, src[cursor++]), RelationalOperator, line, col++);
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
                tokens.emplace_back(std::string(1, src[cursor++]), DoubleQuote, line, col++);
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
                tokens.emplace_back(str, String, line, col);
                col += str.size();

                // If exists, add closing quotation mark
                if(!src.empty() && src[cursor] == '"'){
                    tokens.emplace_back(std::string(1, src[cursor++]), DoubleQuote, line, col++);
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
                    tokens.emplace_back(number, Number, line, col);
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
                    tokens.emplace_back(identifier, reservedPosition->second, line, col);
                }
                else {
                    tokens.emplace_back(identifier, Identifier, line, col);
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
                tokens.emplace_back(snippet, Javascript, line, col);
            }

            else {
                // Something unexpected
                throw std::runtime_error("Unexpected token found at[$line:$col]: '${src[cursor]}'");
            }
        }
    }
    tokens.emplace_back("EOF", EndOfFile, line, col);
    return tokens;
}

Lexer::Lexer() = default;

