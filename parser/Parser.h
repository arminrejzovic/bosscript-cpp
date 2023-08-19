//
// Created by Armin on 19.08.2023.
//

#ifndef BOSSCRIPT_PARSER_H
#define BOSSCRIPT_PARSER_H

#include <queue>
#include <vector>
#include <memory>
#include "AST/Expression/Expression.h"
#include "AST/Statements.h"
#include "../lexer/Lexer.h"

class Parser {
private:
    bool js;
    std::queue<Token> tokens;

    Token current() {
        return tokens.front();
    }

    bool notEOF() {
        return current().type != TokenType::EndOfFile;
    }

    Token consume(){
        Token t = tokens.front();
        tokens.pop();
        return t;
    }

    std::string getCurrentLineCol() {
        return current().getLineCol();
    }

    std::unique_ptr<Expression> assertValidAssignmentTarget(std::unique_ptr<Expression> node);

    Token expect(TokenType expectedType, const std::string& errorMessage);

    void warning(const std::string& message);

    std::vector<std::unique_ptr<Statement>> parseStatementList();

    std::unique_ptr<Statement> parseStatement();

    std::unique_ptr<JavascriptSnippet> parseJavascriptSnippet();

    std::unique_ptr<ModelDefinitionStatement> parseModelDefinitionStatement();

    std::unique_ptr<ModelBlock> parseModelBlock();

    std::unique_ptr<TryCatchStatement> parseTryCatchStatement();

    std::unique_ptr<ImportStatement> parseImportStatement();

    std::unique_ptr<TypeDefinitionStatement> parseTypeDefinitionStatement();

    std::unique_ptr<TypeProperty> parseTypeProperty();

    std::unique_ptr<Expression> parseExpressionStatement();

    std::unique_ptr<WhileStatement> parseWhileStatement();

    std::unique_ptr<DoWhileStatement> parseDoWhileStatement();

    std::unique_ptr<ForStatement> parseForStatement();

    std::unique_ptr<BreakStatement> parseBreakStatement();

    std::unique_ptr<IfStatement> parseIfStatement();

    std::unique_ptr<UnlessStatement> parseUnlessStatement();

    std::unique_ptr<FunctionDeclaration> parseFunctionDeclaration();

    std::unique_ptr<ReturnStatement> parseReturnStatement();

    std::vector<std::unique_ptr<FunctionParameter>> parseFormalParameterList();

    std::unique_ptr<BlockStatement> parseBlockStatement();

    std::unique_ptr<EmptyStatement> parseEmptyStatement();

    std::unique_ptr<VariableStatement> parseVariableStatement();

    std::vector<std::unique_ptr<VariableDeclaration>> parseVariableDeclarationList();

    std::unique_ptr<VariableDeclaration> parseVariableDeclaration();

    std::unique_ptr<Expression> parseVariableInitializer();

    std::unique_ptr<Expression> parseExpression();

    std::unique_ptr<FunctionExpression> parseFunctionExpression();

    std::unique_ptr<Expression> parseAssignmentExpression();

    std::unique_ptr<Expression> parseLogicalOrExpression();

    std::unique_ptr<Expression> parseLogicalAndExpression();

    std::unique_ptr<Expression> parseEqualityExpression();

    std::unique_ptr<Expression> parseRelationalExpression();

    std::unique_ptr<Expression> parseLeftHandSideExpression();

    std::unique_ptr<Expression> parseCallMemberExpression();

    std::unique_ptr<CallExpression> parseCallExpression(std::unique_ptr<Expression> callee);

    std::vector<std::unique_ptr<Expression>> parseArguments();

    std::vector<std::unique_ptr<Expression>> parseArgumentList();

    std::unique_ptr<Expression> parseMemberExpression();

    std::unique_ptr<Identifier> parseIdentifier();

    std::unique_ptr<TypeAnnotation> parseTypeAnnotation();

    std::string parseAssignmentOperator();

    std::unique_ptr<Expression> parseAdditiveExpression();

    std::unique_ptr<Expression> parseMultiplicativeExpression();

    std::unique_ptr<Expression> parseExponentiationExpression();

    std::unique_ptr<Expression> parseUnaryExpression();

    std::unique_ptr<Expression> parsePrimaryExpression();

    std::unique_ptr<NumericLiteral> parseNumericLiteral();

    std::unique_ptr<StringLiteral> parseStringLiteral();

    std::unique_ptr<BooleanLiteral> parseBooleanLiteral();

    std::unique_ptr<NullLiteral> parseNullLiteral();

    std::unique_ptr<Expression> parseParenthesizedExpression();

    std::unique_ptr<ArrayLiteral> parseArrayLiteral();

    std::unique_ptr<ObjectLiteral> parseObjectLiteral();

public:
    explicit Parser(bool js) : js(js) {}

    Parser(bool js, const std::queue<Token> &tokens) : js(js), tokens(tokens) {}

    Program parseProgram(const std::string& src);
};

#endif //BOSSCRIPT_PARSER_H
