//
// Created by Armin on 19.08.2023.
//

#ifndef BOSSCRIPT_EXPRESSION_H
#define BOSSCRIPT_EXPRESSION_H


#include "../Statement/Statement.h"

class Expression : public Statement {
public:
    explicit Expression(NodeType kind);
};


#endif //BOSSCRIPT_EXPRESSION_H
