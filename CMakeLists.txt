cmake_minimum_required(VERSION 3.26)
project(bosscript)

set(CMAKE_CXX_STANDARD 20)

add_executable(bosscript main.cpp
        lexer/Token.cpp
        lexer/Token.h
        lexer/Lexer.cpp
        lexer/Lexer.h
        lexer/TokenType.h
        parser/NodeType.h
        parser/AST/Statement/Statement.cpp
        parser/AST/Statement/Statement.h
        parser/AST/Statements.h
        parser/AST/Expression/Expression.cpp
        parser/AST/Expression/Expression.h
        parser/AST/Expressions.h
        Utils.h
        parser/Parser.cpp
        parser/Parser.h
)
