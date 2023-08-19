//
// Created by Armin on 16.08.2023.
//

#include "Statement.h"

Statement::Statement(NodeType kind) : kind(kind) {}

std::string Statement::toString() {
    return "Generic Statement";
}
