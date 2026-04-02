#ifndef MEDLANG_SERVICE_H
#define MEDLANG_SERVICE_H

#include <string>
#include <vector>

#include "../core/lexer/Lexer.h"
#include "../core/model/HospitalModel.h"
#include "../core/parser/Parser.h"
#include "../core/semantic/SemanticAnalyzer.h"

struct MedLangAnalysisResult {
    bool sourceLoaded;
    bool syntaxOk;
    bool lexicalReportGenerated;
    bool htmlReportsGenerated;

    std::vector<Token> tokens;
    std::vector<LexicalError> lexicalErrors;
    std::vector<SyntaxError> syntaxErrors;

    Hospital hospital;
    SemanticRecognitionResult semanticRecognition;
    SemanticValidationResult semanticValidation;
};

class MedLangService {
public:
    MedLangAnalysisResult analyzeFile(const std::string& filePath) const;
};

#endif
