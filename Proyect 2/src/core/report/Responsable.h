#ifndef WORKLOAD_REPORT_GENERATOR_H
#define WORKLOAD_REPORT_GENERATOR_H

#include "../parser/SyntaxAnalyzer.h"
#include <string>

class WorkloadReportGenerator {
public:
    static void generate(ASTNode* root, const std::string& outPath);
};

#endif
