//
// Created by Armin on 19.08.2023.
//

#ifndef BOSSCRIPT_EXPRESSIONS_H
#define BOSSCRIPT_EXPRESSIONS_H

#include <string>
#include <utility>
#include <vector>
#include "Expression/Expression.h"
#include "Statements.h"


class Identifier: public Expression {
public:
    std::string symbol;

    explicit Identifier(std::string symbol) : Expression(NodeType::Identifier), symbol(std::move(symbol)) {}
};

class FunctionExpression : public Expression {
public:
    std::vector<FunctionParameter> params;
    TypeAnnotation* returnType;
    BlockStatement body;

    FunctionExpression(const std::vector<FunctionParameter> &params, TypeAnnotation *returnType,
                       BlockStatement body) : Expression(NodeType::FunctionExpression), params(params), returnType(returnType),
                                                     body(std::move(body)) {}
};

class AssignmentExpression : public Expression {
public:
    Expression assignee;
    Expression value;
    std::string assignmentOperator;

    AssignmentExpression(const Expression &assignee, const Expression &value,
                         std::string assignmentOperator) : Expression(NodeType::AssignmentExpression), assignee(assignee), value(value),
                                                                  assignmentOperator(std::move(assignmentOperator)) {}
};

class MemberExpression : public Expression {
public:
    bool isComputed;
    Expression targetObject;
    Expression property;

    MemberExpression(bool isComputed, const Expression &targetObject, const Expression &aProperty)
            : Expression(NodeType::MemberExpression), isComputed(isComputed), targetObject(targetObject), property(aProperty) {}
};

class LogicalExpression : public Expression {
public:
    Expression left;
    Expression right;
    std::string mOperator;

    LogicalExpression(const Expression &left, const Expression &right, std::string mOperator)
            : Expression(NodeType::LogicalExpression), left(left), right(right), mOperator(std::move(mOperator)) {}
};

class JavascriptSnippet : public Expression {
public:
    std::string code;

    explicit JavascriptSnippet(std::string code) : Expression(NodeType::Javascript), code(std::move(code)) {}
};

class BinaryExpression : public Expression {
public:
    Expression left;
    Expression right;
    std::string mOperator;

    BinaryExpression(const Expression &left, const Expression &right, std::string mOperator)
            : Expression(NodeType::BinaryExpression), left(left), right(right), mOperator(std::move(mOperator)) {}
};

class UnaryExpression : public Expression {
public:
    std::string mOperator;
    Expression operand;

    UnaryExpression(std::string mOperator, const Expression &operand)
        : Expression(NodeType::UnaryExpression), mOperator(std::move(mOperator)), operand(operand) {}
};

class NumericLiteral : public Expression {
public:
    double value;

    explicit NumericLiteral(double value) : Expression(NodeType::NumericLiteral), value(value) {}
};

class StringLiteral : public Expression {
public:
    std::string value;

    explicit StringLiteral(std::string value) : Expression(NodeType::StringLiteral), value(std::move(value)) {}
};

class BooleanLiteral : public Expression {
public:
    bool value;

    explicit BooleanLiteral(bool value) : Expression(NodeType::BooleanLiteral), value(value) {}
};

class NullLiteral : public Expression {
public:
    void* value;

    NullLiteral() : Expression(NodeType::NullLiteral), value(nullptr) {}
};


class ObjectProperty : public Expression {
public:
    std::string key;
    Expression value;

    ObjectProperty(std::string key, const Expression &value)
        : Expression(NodeType::ObjectProperty), key(std::move(key)), value(value) {}
};

class ObjectLiteral : public Expression {
public:
    std::vector<ObjectProperty> properties;

    explicit ObjectLiteral(const std::vector<ObjectProperty> &properties) : Expression(NodeType::Object),
                                                                                  properties(properties) {}
};

class ArrayLiteral : public Expression {
public:
    std::vector<Expression> arr;

    explicit ArrayLiteral(const std::vector<Expression> &arr) : Expression(NodeType::ArrayLiteral), arr(arr) {}
};

class CallExpression : public Expression {
public:
    std::vector<Expression> args;
    Expression callee;

    CallExpression(const std::vector<Expression> &args, const Expression &callee)
        : Expression(NodeType::CallExpression), args(args), callee(callee) {}
};

#endif //BOSSCRIPT_EXPRESSIONS_H
