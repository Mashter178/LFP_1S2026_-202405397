#include "MedLangService.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "../core/report/Generator.h"
#include "../core/report/Html.h"
#include "../core/report/LexicalReport.h"

namespace {
std::string buildEarlyIndexBody(const MedLangAnalysisResult& result) {
    std::ostringstream body;
    body << "<div class=\"card\">";
    body << "<h2 class=\"section-title\">Indice de reportes (parcial)</h2>";
    body << "<p class=\"muted\">Se detectaron errores lexicos o sintacticos. Se muestran los reportes disponibles.</p>";
    body << "<table><thead><tr><th>#</th><th>Reporte</th><th>Archivo</th><th>Estado</th></tr></thead><tbody>";
    body << "<tr><td>1</td><td>Reporte Lexico</td><td><a href=\"reporte_lexico.html\">reporte_lexico.html</a></td><td>";
    body << (result.lexicalReportGenerated ? "Generado" : "No generado");
    body << "</td></tr>";
    body << "</tbody></table>";
    body << "</div>";
    return body.str();
}

bool writeFallbackIndex(const MedLangAnalysisResult& result) {
    std::filesystem::create_directories("output");
    return writeHtmlDocument(
        "output/indice_reportes.html",
        "Indice de Reportes MedLang",
        buildEarlyIndexBody(result));
}
}

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
    std::filesystem::create_directories("output");
    result.lexicalReportGenerated = writeLexicalHtmlReport("output/reporte_lexico.html", result.tokens, result.lexicalErrors);

    Parser parser(result.tokens);
    result.syntaxOk = parser.parseProgram();
    result.syntaxErrors = parser.getErrors();
    result.hospital = parser.getHospitalData();

    if (!result.syntaxOk) {
        result.htmlReportsGenerated = writeFallbackIndex(result);
        return result;
    }

    SemanticAnalyzer semanticAnalyzer(result.hospital);
    result.semanticRecognition = semanticAnalyzer.recognizeInput();
    result.semanticValidation = semanticAnalyzer.validateBasicRules();

    result.htmlReportsGenerated = Generator::generateAllHtmlReports("output", result.hospital, result.semanticValidation, filePath);
    if (!result.htmlReportsGenerated) {
        result.htmlReportsGenerated = writeFallbackIndex(result);
    }
    return result;
}
