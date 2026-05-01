#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "../core/lexer/LexicalAnalyzer.h"
#include "../core/parser/SyntaxAnalyzer.h"
#include "../core/report/ReportGenerator.h"
#include "../core/semantic/SemanticAnalyzer.h"

static std::string readFile(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        return {};
    }
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: taskscript_cli archivo.task\n";
        return 1;
    }

    const std::string inputPath = argv[1];
    const std::string source = readFile(inputPath);
    if (source.empty()) {
        std::cerr << "No se pudo abrir el archivo: " << inputPath << "\n";
        return 1;
    }

    LexicalAnalyzer lexerForTokens(source);
    std::vector<Token> tokens;
    while (true) {
        Token token = lexerForTokens.nextToken();
        tokens.push_back(token);
        if (token.type == TokenType::EndOfFile) {
            break;
        }
    }

    LexicalAnalyzer lexerForParser(source);
    SyntaxAnalyzer parser(lexerForParser);
    parser.parse();

    SemanticAnalyzer sema;
    auto semErrors = sema.analyze(parser.getRoot());

    std::filesystem::create_directories("output");
    ReportGenerator::generateTokenReport(tokens, "output/tokens.html");
    ReportGenerator::generateErrorReport(
        lexerForTokens.getErrors(),
        parser.getErrors(),
        semErrors,
        "output/errors.html"
    );
    ReportGenerator::generateStatsReport(
        tokens,
        lexerForTokens.getErrors(),
        parser.getErrors(),
        semErrors,
        "output/stats.html"
    );
    ReportGenerator::generateKanbanReport(parser.getRoot(), "output/kanban.html");
    ReportGenerator::generateWorkloadReport(parser.getRoot(), "output/workload.html");
    ReportGenerator::generateAstDot(parser.getRoot(), "output/ast.dot");

    std::cout << "Analisis completado. Reportes generados en ./output\n";
    return 0;
}
