//
// Created by Armin on 16.08.2023.
//

#ifndef BOSSCRIPT_STATEMENTS_H
#define BOSSCRIPT_STATEMENTS_H

#include "Statement/Statement.h"
#include "Expressions.h"
#include "../../Utils.h"
#include <utility>
#include <vector>
#include <string>
#include <optional>
#include <memory>

class EmptyStatement : public Statement {
public:
    explicit EmptyStatement() : Statement(NodeType::EmptyStatement){}
};

class BreakStatement : public Statement {
public:
    explicit BreakStatement() : Statement(NodeType::BreakStatement){}
};

class Program : public Statement {
public:
    std::vector<Statement> body;

    explicit Program(const std::vector<Statement>& body) : Statement(NodeType::Program), body(body){}
};

class BlockStatement : public Statement {
public:
    std::vector<Statement> body;

    explicit BlockStatement(const std::vector<Statement>& body) : Statement(NodeType::Block), body(body){}
};

class TryCatchStatement : public Statement {
public:
    BlockStatement tryBlock;
    BlockStatement catchBlock;
    BlockStatement* finallyBlock;

    TryCatchStatement(NodeType kind, BlockStatement tryBlock, BlockStatement catchBlock, BlockStatement* finallyBlock)
            : Statement(kind), tryBlock(std::move(tryBlock)), catchBlock(std::move(catchBlock)), finallyBlock(finallyBlock) {}
};

class TypeAnnotation : public Statement {
public:
    std::string typeName;
    bool isArrayType;

    TypeAnnotation(std::string typeName, bool isArrayType)
        : Statement(NodeType::TypeAnnotation), typeName(std::move(typeName)), isArrayType(isArrayType) {}
};

class FunctionParameter : public Statement {
public:
    Identifier identifier;
    TypeAnnotation* typeAnnotation;

    FunctionParameter(Identifier identifier, TypeAnnotation* typeAnnotation)
        : Statement(NodeType::FunctionParameter), identifier(std::move(identifier)), typeAnnotation(typeAnnotation) {}
};

class FunctionDeclaration : public Statement {
public:
    Identifier name;
    std::vector<FunctionParameter> params;
    TypeAnnotation* returnType;
    BlockStatement body;

    FunctionDeclaration(Identifier name, const std::vector<FunctionParameter> &params, TypeAnnotation* returnType, BlockStatement body)
        : Statement(NodeType::FunctionDeclaration),
            name(std::move(name)),
            params(params),
            returnType(returnType),
            body(std::move(body)){}
};

class ReturnStatement : public Statement {
public:
    Expression* argument;

    explicit ReturnStatement(Expression* argument) : Statement(NodeType::ReturnStatement), argument(argument) {}
};

class IfStatement : public Statement {
public:
    Expression condition;
    Statement consequent;
    Statement* alternate;

    IfStatement(NodeType kind, const Expression &condition, const Statement &consequent, Statement* alternate)
            : Statement(kind), condition(condition), consequent(consequent), alternate(alternate) {}
};

class WhileStatement : public Statement {
    Expression condition;
    BlockStatement body;

public:
    WhileStatement(const Expression &condition, BlockStatement body)
        : Statement(NodeType::WhileStatement), condition(condition), body(std::move(body)) {}
};

class DoWhileStatement : public Statement {
    Expression condition;
    BlockStatement body;

public:
    DoWhileStatement(const Expression &condition, BlockStatement body)
            : Statement(NodeType::DoWhileStatement), condition(condition), body(std::move(body)) {}
};

class ForStatement : public Statement {
    Identifier counter;
    Expression startValue;
    Expression endValue;
    Expression* step;
    BlockStatement body;

public:
    ForStatement(Identifier counter, const Expression &startValue, const Expression &endValue,
                 Expression *step, BlockStatement body) : Statement(NodeType::ForStatement), counter(std::move(counter)),
                                                                 startValue(startValue), endValue(endValue), step(step),
                                                                 body(std::move(body)) {}
};

class UnlessStatement : public Statement {
public:
    Expression condition;
    Statement consequent;
    Statement* alternate;

    UnlessStatement(const Expression &condition, const Statement &consequent, Statement* alternate)
            : Statement(NodeType::IfStatement), condition(condition), consequent(consequent), alternate(alternate) {}
};

class VariableDeclaration : public Statement {
    std::string name;
    Expression* value;

public:
    VariableDeclaration(std::string name, Expression *value)
        : Statement(NodeType::VariableDeclaration), name(std::move(name)), value(value) {}
};

class VariableStatement : public Statement {
public:
    std::vector<VariableDeclaration> declarations;
    bool isConstant;

    explicit VariableStatement(const std::vector<VariableDeclaration> &declarations, bool isConstant = false)
        : Statement(NodeType::VariableStatement), declarations(declarations), isConstant(isConstant) {}
};

class TypeProperty : public Statement {
    std::string name;
    TypeAnnotation type;

public:
    TypeProperty(std::string name, TypeAnnotation type)
        : Statement(NodeType::TypePropertyDefinition), name(std::move(name)), type(std::move(type)) {}
};

class TypeDefinitionStatement : public Statement {
public:
    Identifier name;
    Identifier* parentTypeName;
    std::vector<TypeProperty> properties;

    TypeDefinitionStatement(Identifier name, Identifier *parentTypeName,
                            const std::vector<TypeProperty> &properties) : Statement(NodeType::TypeDefinition), name(std::move(name)),
                                                                           parentTypeName(parentTypeName),
                                                                           properties(properties) {}
};

class ModelBlock : public Statement {
private:
    std::vector<Statement> body;

public:
    std::vector<Statement> getBody(){
        return body;
    }

    void addStatement(Statement stmt){
        if((stmt.kind == NodeType::VariableStatement) or (stmt.kind == NodeType::FunctionDeclaration)){
            body.emplace_back(stmt);
        }
        throw std::runtime_error("Expected member declaration");
    }
};

class ModelDefinitionStatement : public Statement {
public:
    Identifier className;
    Identifier* parentClassName;
    FunctionDeclaration constructor;
    ModelBlock* privateBlock;
    ModelBlock* publicBlock;

    ModelDefinitionStatement(Identifier className, Identifier *parentClassName,
                             FunctionDeclaration constructor, ModelBlock *privateBlock, ModelBlock *publicBlock)
            : Statement(NodeType::ModelDefinition), className(std::move(className)), parentClassName(parentClassName), constructor(std::move(constructor)),
              privateBlock(privateBlock), publicBlock(publicBlock) {}
};

class ImportStatement : public Statement {
public:
    std::string packageName;
    std::vector<Identifier>* imports;
    ImportStatement(std::string packageName, std::vector<Identifier> *imports)
        : Statement(NodeType::ImportStatement), packageName(std::move(packageName)), imports(imports) {}
};



#endif //BOSSCRIPT_STATEMENTS_H
