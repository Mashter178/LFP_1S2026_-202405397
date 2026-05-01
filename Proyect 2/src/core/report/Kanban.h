#ifndef KANBAN_REPORT_GENERATOR_H
#define KANBAN_REPORT_GENERATOR_H

#include "../parser/SyntaxAnalyzer.h"
#include <string>

class KanbanReportGenerator {
public:
    static void generate(ASTNode* root, const std::string& outPath);
};

#endif
