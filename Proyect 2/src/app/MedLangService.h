#ifndef MEDLANG_SERVICE_H
#define MEDLANG_SERVICE_H

#include <string>
#include <vector>

#include "../core/lexer/Token.h"
#include "../core/lexer/LexicalAnalyzer.h"
#include "../core/parser/SyntaxAnalyzer.h"
#include "../core/semantic/SemanticAnalyzer.h"

struct SemanticValidationResult {
    std::vector<SemanticError> errors;
};

struct MedLangAnalysisResult {
    bool sourceLoaded{false};
    bool syntaxOk{false};
    bool lexicalReportGenerated{false};
    bool htmlReportsGenerated{false};

    std::vector<Token> tokens;
    std::vector<LexicalError> lexicalErrors;
    std::vector<SyntaxError> syntaxErrors;

    SemanticValidationResult semanticValidation;
};

class MedLangService {
public:
    MedLangAnalysisResult analyzeFile(const std::string& filePath, bool forceGenerateReports = false) const;
};

#endif