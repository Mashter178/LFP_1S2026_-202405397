#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include <string>
#include <vector>

#include "model/HospitalModel.h"

struct SemanticRecognitionResult {
    bool inputReady;
    std::vector<std::string> notes;
};

struct SemanticError {
    int number;
    std::string type;
    std::string description;
    std::string entity;
    int line;
};

struct SemanticValidationResult {
    bool valid;
    std::vector<SemanticError> errors;
};

class SemanticAnalyzer {
public:
    explicit SemanticAnalyzer(const HospitalData& data);

    SemanticRecognitionResult recognizeInput() const;
    SemanticValidationResult validateBasicRules() const;

private:
    const HospitalData& m_data;
};

#endif
