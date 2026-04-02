#include "MedLangService.h"

#include <fstream>
#include <sstream>

#include "../core/report/Generator.h"
#include "../core/report/LexicalReport.h"

MedLangAnalysisResult MedLangService::analyzeFile(const std::string& filePath) const {
    MedLangAnalysisResult result;
    result.sourceLoaded = false;
    result.syntaxOk = false;
    result.lexicalReportGenerated = false;
    result.htmlReportsGenerated = false;

    std::ifstream in(filePath.c_str(), std::ios::in);
    if (!in) {
        return result;
    }

    result.sourceLoaded = true;

    std::ostringstream buffer;
    buffer << in.rdbuf();
    const std::string source = buffer.str();

    Lexer lexer(source);
    while (true) {
        Token token = lexer.nextToken();
        result.tokens.push_back(token);
        if (token.type == TokenType::EndOfFile) {
            break;
        }
    }

    result.lexicalErrors = lexer.getErrors();
    result.lexicalReportGenerated = writeLexicalHtmlReport("reporte_lexico.html", result.tokens, result.lexicalErrors);

    Parser parser(result.tokens);
    result.syntaxOk = parser.parseProgram();
    result.syntaxErrors = parser.getErrors();
    result.hospital = parser.getHospitalData();

    if (!result.syntaxOk) {
        return result;
    }

    SemanticAnalyzer semanticAnalyzer(result.hospital);
    result.semanticRecognition = semanticAnalyzer.recognizeInput();
    result.semanticValidation = semanticAnalyzer.validateBasicRules();

    result.htmlReportsGenerated = Generator::generateAllHtmlReports("output", result.hospital, result.semanticValidation);
    return result;
}
