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
    std::vector<std::unique_ptr<Statement>> body;

    explicit Program(std::vector<std::unique_ptr<Statement>> body) : Statement(NodeType::Program), body(std::move(body)) {}
};


class BlockStatement : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> body;

    explicit BlockStatement(std::vector<std::unique_ptr<Statement>> body) : Statement(NodeType::Block), body(std::move(body)) {}
};

class TryCatchStatement : public Statement {
public:
    std::unique_ptr<BlockStatement> tryBlock;
    std::unique_ptr<BlockStatement> catchBlock;
    std::unique_ptr<BlockStatement> finallyBlock;

    TryCatchStatement(std::unique_ptr<BlockStatement> tryBlock, std::unique_ptr<BlockStatement> catchBlock, std::unique_ptr<BlockStatement> finallyBlock)
            : Statement(NodeType::TryCatch), tryBlock(std::move(tryBlock)), catchBlock(std::move(catchBlock)), finallyBlock(std::move(finallyBlock)) {}
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
    std::unique_ptr<Identifier> identifier;
    std::unique_ptr<TypeAnnotation> typeAnnotation;

    FunctionParameter(std::unique_ptr<Identifier> identifier, std::unique_ptr<TypeAnnotation> typeAnnotation)
        : Statement(NodeType::FunctionParameter), identifier(std::move(identifier)), typeAnnotation(std::move(typeAnnotation)) {}
};

class FunctionDeclaration : public Statement {
public:
    std::unique_ptr<Identifier> name;
    std::vector<std::unique_ptr<FunctionParameter>> params;
    std::unique_ptr<TypeAnnotation> returnType;
    std::unique_ptr<BlockStatement> body;

    FunctionDeclaration(std::unique_ptr<Identifier> name,
                        std::vector<std::unique_ptr<FunctionParameter>> &params,
                        std::unique_ptr<TypeAnnotation> returnType, std::unique_ptr<BlockStatement> body)
            : Statement(NodeType::FunctionDeclaration), name(std::move(name)), params(std::move(params)), returnType(std::move(returnType)), body(std::move(body)) {}

};

class FunctionExpression : public Expression {
public:
    std::vector<std::unique_ptr<FunctionParameter>> params;
    std::unique_ptr<TypeAnnotation> returnType;
    std::unique_ptr<BlockStatement> body;

    FunctionExpression(std::vector<std::unique_ptr<FunctionParameter>> &params,
                       std::unique_ptr<TypeAnnotation> returnType, std::unique_ptr<BlockStatement> body)
            : Expression(NodeType::FunctionExpression), params(std::move(params)), returnType(std::move(returnType)), body(std::move(body)) {}

};

class ReturnStatement : public Statement {
public:
    std::unique_ptr<Expression> argument;

    explicit ReturnStatement(std::unique_ptr<Expression> argument) : Statement(NodeType::ReturnStatement), argument(std::move(argument)) {}
};

class IfStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> consequent;
    std::unique_ptr<Statement> alternate;

    IfStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> consequent, std::unique_ptr<Statement> alternate)
        : Statement(NodeType::IfStatement), condition(std::move(condition)), consequent(std::move(consequent)), alternate(std::move(alternate)) {}

};

class WhileStatement : public Statement {
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> body;

public:
    WhileStatement(std::unique_ptr<Expression> condition, std::unique_ptr<BlockStatement> body)
        : Statement(NodeType::WhileStatement), condition(std::move(condition)), body(std::move(body)) {}
};

class DoWhileStatement : public Statement {
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> body;

public:
    DoWhileStatement(std::unique_ptr<Expression> condition, std::unique_ptr<BlockStatement> body)
            : Statement(NodeType::DoWhileStatement), condition(std::move(condition)), body(std::move(body)) {}
};

class ForStatement : public Statement {
    std::unique_ptr<Identifier> counter;
    std::unique_ptr<Expression> startValue;
    std::unique_ptr<Expression> endValue;
    std::unique_ptr<Expression> step;
    std::unique_ptr<BlockStatement> body;

public:
    ForStatement(std::unique_ptr<Identifier> counter, std::unique_ptr<Expression> startValue, std::unique_ptr<Expression> endValue, std::unique_ptr<Expression> step, std::unique_ptr<BlockStatement> body)
         : Statement(NodeType::ForStatement),
            counter(std::move(counter)),
            startValue(std::move(startValue)),
            endValue(std::move(endValue)),
            step(std::move(step)),
            body(std::move(body))
         {}
};

class UnlessStatement : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> consequent;
    std::unique_ptr<Statement> alternate;

    UnlessStatement(std::unique_ptr<Expression> condition, std::unique_ptr<Statement> consequent, std::unique_ptr<Statement> alternate)
            : Statement(NodeType::IfStatement), condition(std::move(condition)), consequent(std::move(consequent)), alternate(std::move(alternate)) {}
};

class VariableDeclaration : public Statement {
    std::string name;
    std::unique_ptr<Expression> value;

public:
    VariableDeclaration(std::string name, std::unique_ptr<Expression> value)
        : Statement(NodeType::VariableDeclaration), name(std::move(name)), value(std::move(value)) {}
};

class VariableStatement : public Statement {
public:
    std::vector<std::unique_ptr<VariableDeclaration>> declarations;
    bool isConstant;

    explicit VariableStatement(std::vector<std::unique_ptr<VariableDeclaration>> declarations, bool isConstant = false)
        : Statement(NodeType::VariableStatement), declarations(std::move(declarations)), isConstant(isConstant) {}
};

class TypeProperty : public Statement {
    std::string name;
    std::unique_ptr<TypeAnnotation> type;

public:
    TypeProperty(std::string name, std::unique_ptr<TypeAnnotation> type)
        : Statement(NodeType::TypePropertyDefinition), name(std::move(name)), type(std::move(type)) {}
};

class TypeDefinitionStatement : public Statement {
public:
    std::unique_ptr<Identifier> name;
    std::unique_ptr<Identifier> parentTypeName;
    std::vector<std::unique_ptr<TypeProperty>> properties;

    TypeDefinitionStatement(std::unique_ptr<Identifier> name, std::unique_ptr<Identifier> parentTypeName, std::vector<std::unique_ptr<TypeProperty>> &properties)
        : Statement(NodeType::TypeDefinition), name(std::move(name)), parentTypeName(std::move(parentTypeName)), properties(std::move(properties)) {}
};

class ModelBlock : public Statement {
private:
    std::vector<std::unique_ptr<Statement>> body;

public:
    std::vector<std::unique_ptr<Statement>> getBody(){
        return std::move(body);
    }

    void addStatement(std::unique_ptr<Statement> stmt){
        if((stmt->kind == NodeType::VariableStatement) or (stmt->kind == NodeType::FunctionDeclaration)){
            body.emplace_back(std::move(stmt));
        }
        throw std::runtime_error("Expected member declaration");
    }
};

class ModelDefinitionStatement : public Statement {
public:
    std::unique_ptr<Identifier> className;
    std::unique_ptr<Identifier> parentClassName;
    std::unique_ptr<FunctionDeclaration> constructor;
    std::unique_ptr<ModelBlock> privateBlock;
    std::unique_ptr<ModelBlock> publicBlock;

    ModelDefinitionStatement(std::unique_ptr<Identifier> className, std::unique_ptr<Identifier> parentClassName, std::unique_ptr<FunctionDeclaration> constructor, std::unique_ptr<ModelBlock> privateBlock, std::unique_ptr<ModelBlock> publicBlock)
            : Statement(NodeType::ModelDefinition),
                className(std::move(className)),
                parentClassName(std::move(parentClassName)),
                constructor(std::move(constructor)),
                privateBlock(std::move(privateBlock)),
                publicBlock(std::move(publicBlock))
            {}
};

class ImportStatement : public Statement {
public:
    std::string packageName;
    std::vector<std::unique_ptr<Identifier>> imports;
    ImportStatement(std::string packageName, std::vector<std::unique_ptr<Identifier>> imports)
        : Statement(NodeType::ImportStatement), packageName(std::move(packageName)), imports(std::move(imports)) {}
};



#endif //BOSSCRIPT_STATEMENTS_H
