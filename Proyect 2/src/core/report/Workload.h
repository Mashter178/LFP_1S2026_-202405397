#ifndef WORKLOAD_H
#define WORKLOAD_H

#include "../parser/SyntaxAnalyzer.h"
#include <string>

class Workload {
public:
    static void generate(ASTNode* root, const std::string& outPath);
};

#endif
