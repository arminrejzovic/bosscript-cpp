//
// Created by Armin on 16.08.2023.
//

#ifndef BOSSCRIPT_STATEMENT_H
#define BOSSCRIPT_STATEMENT_H


#include <string>
#include "../../NodeType.h"

class Statement {
public:
   NodeType kind;

    explicit Statement(NodeType kind);

    virtual std::string toString();
};


#endif //BOSSCRIPT_STATEMENT_H
