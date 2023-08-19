//
// Created by Armin on 19.08.2023.
//

#ifndef BOSSCRIPT_EXPRESSIONS_H
#define BOSSCRIPT_EXPRESSIONS_H

#include <string>
#include <utility>
#include <vector>
#include "Expression/Expression.h"

class Identifier: public Expression {
public:
    std::string symbol;

    explicit Identifier(std::string symbol) : Expression(NodeType::Identifier), symbol(std::move(symbol)) {}
};

class AssignmentExpression : public Expression {
public:
    std::unique_ptr<Expression> assignee;
    std::unique_ptr<Expression> value;
    std::string assignmentOperator;

    AssignmentExpression(std::unique_ptr<Expression> assignee, std::unique_ptr<Expression> value, std::string assignmentOperator)
        : Expression(NodeType::AssignmentExpression),
            assignee(std::move(assignee)),
            value(std::move(value)),
            assignmentOperator(std::move(assignmentOperator))
        {}
};

class MemberExpression : public Expression {
public:
    bool isComputed;
    std::unique_ptr<Expression> targetObject;
    std::unique_ptr<Expression> property;

    MemberExpression(bool isComputed, std::unique_ptr<Expression> targetObject, std::unique_ptr<Expression> property)
        : Expression(NodeType::MemberExpression),
            isComputed(isComputed),
            targetObject(std::move(targetObject)),
            property(std::move(property))
        {}
};

class LogicalExpression : public Expression {
public:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    std::string mOperator;

    LogicalExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, std::string mOperator)
        : Expression(NodeType::LogicalExpression),
            left(std::move(left)),
            right(std::move(right)),
            mOperator(std::move(mOperator))
        {}
};

class JavascriptSnippet : public Expression {
public:
    std::string code;

    explicit JavascriptSnippet(std::string code) : Expression(NodeType::Javascript), code(std::move(code)) {}
};

class BinaryExpression : public Expression {
public:
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    std::string mOperator;

    BinaryExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right, std::string mOperator)
        : Expression(NodeType::LogicalExpression),
            left(std::move(left)),
            right(std::move(right)),
            mOperator(std::move(mOperator))
        {}
};

class UnaryExpression : public Expression {
public:
    std::string mOperator;
    std::unique_ptr<Expression> operand;

    UnaryExpression(std::string mOperator, std::unique_ptr<Expression> operand)
        : Expression(NodeType::UnaryExpression),
            mOperator(std::move(mOperator)),
            operand(std::move(operand))
        {}
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
    std::unique_ptr<Expression> value;

    ObjectProperty(std::string key, std::unique_ptr<Expression> value)
        : Expression(NodeType::ObjectProperty),
            key(std::move(key)),
            value(std::move(value))
        {}
};

class ObjectLiteral : public Expression {
public:
    std::vector<std::unique_ptr<ObjectProperty>> properties;

    explicit ObjectLiteral(std::vector<std::unique_ptr<ObjectProperty>> &properties)
        : Expression(NodeType::Object),
            properties(std::move(properties))
        {}
};

class ArrayLiteral : public Expression {
public:
    std::vector<std::unique_ptr<Expression>> arr;

    explicit ArrayLiteral(std::vector<std::unique_ptr<Expression>> arr)
        : Expression(NodeType::ArrayLiteral),
            arr(std::move(arr))
        {}
};

class CallExpression : public Expression {
public:
    std::vector<std::unique_ptr<Expression>> args;
    std::unique_ptr<Expression> callee;

    CallExpression(std::vector<std::unique_ptr<Expression>> args, std::unique_ptr<Expression> callee)
        : Expression(NodeType::CallExpression),
            args(std::move(args)),
            callee(std::move(callee))
        {}
};

#endif //BOSSCRIPT_EXPRESSIONS_H
