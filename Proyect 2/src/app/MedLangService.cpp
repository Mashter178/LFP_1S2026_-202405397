#include "MedLangService.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "../core/report/Report.h"

namespace {
std::string readFile(const std::string& filePath) {
    std::ifstream input(filePath, std::ios::in | std::ios::binary);
    if (!input.is_open()) {
        return {};
    }

    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

void writeReportIndex(const std::filesystem::path& outputDir) {
    std::ofstream index(outputDir / "indice_reportes.html");
    index << "<!DOCTYPE html><html><head><meta charset=\"utf-8\">";
    index << "<title>Indice de reportes</title>";
    index << "<style>body{font-family:Segoe UI,sans-serif;padding:30px;background:#f4f7fb}ul{line-height:1.8}a{color:#1e5aa8;text-decoration:none}</style>";
    index << "</head><body><h1>Indice de reportes</h1><ul>";
    index << "<li><a href=\"reporte_kanban.html\">Reporte Kanban</a></li>";
    index << "<li><a href=\"reporte_carga.html\">Reporte de carga</a></li>";
    index << "<li><a href=\"ast.dot\">AST en DOT</a></li>";
    index << "</ul></body></html>";
}
} // namespace

MedLangAnalysisResult MedLangService::analyzeFile(const std::string& filePath, bool forceGenerateReports) const {
    MedLangAnalysisResult result;

    const std::string source = readFile(filePath);
    if (source.empty()) {
        return result;
    }

    result.sourceLoaded = true;

    LexicalAnalyzer lexicalAnalyzer(source);
    while (true) {
        const Token token = lexicalAnalyzer.nextToken();
        if (token.type == TokenType::EndOfFile) {
            break;
        }
    }

    result.tokens = lexicalAnalyzer.getTokens();
    result.lexicalErrors = lexicalAnalyzer.getErrors();
    if (!result.tokens.empty() && result.tokens.back().type == TokenType::EndOfFile) {
        result.tokens.pop_back();
    }

    lexicalAnalyzer.reset();
    SyntaxAnalyzer syntaxAnalyzer(lexicalAnalyzer);
    syntaxAnalyzer.parse();

    result.syntaxErrors = syntaxAnalyzer.getErrors();
    result.syntaxOk = result.syntaxErrors.empty();

    ASTNode* root = syntaxAnalyzer.getRoot();
    SemanticAnalyzer semanticAnalyzer;
    result.semanticValidation.errors = semanticAnalyzer.analyze(root);

    if (forceGenerateReports) {
        const std::filesystem::path outputDir = std::filesystem::current_path() / "output";
        std::filesystem::create_directories(outputDir);

        Report::generateKanbanReport(root, (outputDir / "reporte_kanban.html").string());
        Report::generateWorkloadReport(root, (outputDir / "reporte_carga.html").string());
        Report::generateAstDot(root, (outputDir / "ast.dot").string());
        writeReportIndex(outputDir);
        result.htmlReportsGenerated = true;
        result.lexicalReportGenerated = true;
    }

    return result;
}