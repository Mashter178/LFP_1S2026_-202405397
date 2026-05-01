#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include "../lexer/Token.h"
#include "../lexer/LexicalAnalyzer.h"
#include "../parser/SyntaxAnalyzer.h"
#include "../semantic/SemanticAnalyzer.h"
#include <string>

// Fachada central para generar todos los reportes
class ReportGenerator {
public:
    // Reportes HTML
    static void generateTokenReport(const std::vector<Token>& tokens, const std::string& outPath);
    static void generateErrorReport(const std::vector<LexicalError>& lexErrors, const std::vector<SyntaxError>& synErrors, const std::vector<SemanticError>& semErrors, const std::string& outPath);
    static void generateStatsReport(const std::vector<Token>& tokens, const std::vector<LexicalError>& lexErrors, const std::vector<SyntaxError>& synErrors, const std::vector<SemanticError>& semErrors, const std::string& outPath);
    
    // Reportes visuales especializados
    static void generateKanbanReport(ASTNode* root, const std::string& outPath);
    static void generateWorkloadReport(ASTNode* root, const std::string& outPath);
    
    // Otros formatos
    static void generateAstDot(ASTNode* root, const std::string& outPath);
};

#endif
