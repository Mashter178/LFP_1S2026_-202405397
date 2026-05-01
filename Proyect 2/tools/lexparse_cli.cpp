#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include "../src/core/lexer/LexicalAnalyzer.h"
#include "../src/core/parser/SyntaxAnalyzer.h"
#include "../src/core/report/ReportGenerator.h"

int main(int argc, char** argv) {
    if (argc < 2) { std::cerr<<"Uso: lexparse_cli <archivo.task>\n"; return 1; }
    std::string path = argv[1];
    std::ifstream in(path);
    if (!in) { std::cerr<<"No se pudo abrir "<<path<<"\n"; return 1; }
    std::ostringstream ss; ss << in.rdbuf();
    std::string content = ss.str();

    LexicalAnalyzer lexer(content);
    std::vector<Token> tokens;
    while (true) {
        Token t = lexer.nextToken();
        tokens.push_back(t);
        if (t.type == TokenType::EndOfFile) break;
    }

    // parser
    SyntaxAnalyzer parser(lexer);
    parser.parse();

    // prepare output folder
    std::filesystem::create_directories("output");
    ReportGenerator::generateTokenReport(tokens, "output/tokens.html");
    ReportGenerator::generateErrorReport(lexer.getErrors(), parser.getErrors(), "output/errors.html");
    ReportGenerator::generateStatsReport(tokens, lexer.getErrors(), parser.getErrors(), "output/stats.html");
    ReportGenerator::generateAstDot(parser.getRoot(), "output/ast.dot");

    std::cout<<"Reportes generados en ./output\n";
    return 0;
}
