#ifndef BOSSCRIPT_NODETYPE_H
#define BOSSCRIPT_NODETYPE_H
#endif //BOSSCRIPT_NODETYPE_H


enum class NodeType{
    Program,
    Block,
    VariableDeclaration,
    Identifier,
    BinaryExpression,
    AssignmentExpression,
    MemberExpression,
    CallExpression,
    NumericLiteral,
    StringLiteral,
    Object,
    ObjectProperty,
    EmptyStatement,
    VariableStatement,
    IfStatement,
    UnlessStatement,
    BooleanLiteral,
    NullLiteral,
    LogicalExpression,
    UnaryExpression,
    WhileStatement,
    DoWhileStatement,
    ForStatement,
    FunctionDeclaration,
    ReturnStatement,
    TypeDefinition,
    TypePropertyDefinition,
    TypeAnnotation,
    FunctionParameter,
    ArrayLiteral,
    FunctionExpression,
    BreakStatement,
    ImportStatement,
    TryCatch,
    ModelDefinition,
    ModelBlock,
    Javascript
};