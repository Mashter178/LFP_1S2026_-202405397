#ifndef KANBAN_H
#define KANBAN_H

#include "../parser/SyntaxAnalyzer.h"
#include <string>

class Kanban {
public:
    static void generate(ASTNode* root, const std::string& outPath);
};

#endif
