cmake_minimum_required(VERSION 3.26)
project(bosscript)

set(CMAKE_CXX_STANDARD 17)

add_executable(bosscript main.cpp
        lexer/Token.cpp
        lexer/Token.h
        lexer/Lexer.cpp
        lexer/Lexer.h
        lexer/TokenType.h
)
