#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "../parser/SyntaxAnalyzer.h"
#include <string>
#include <vector>

struct SemanticError {
    int number;
    std::string lexeme;
    std::string errorType;
    std::string description;
    int line;
    int column;
};

class SemanticAnalyzer {
public:
    SemanticAnalyzer();
    std::vector<SemanticError> analyze(ASTNode* root);

private:
    std::vector<SemanticError> m_errors;
    int m_counter{0};

    void analyzeProgram(ASTNode* program);
    void analyzeBoard(ASTNode* board);
    void analyzeColumn(ASTNode* column);
    void analyzeTask(ASTNode* task);
    void addError(const std::string& lex, const std::string& desc, int line, int col);
};

#endif
