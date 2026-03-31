#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "reporting/LexicalReport.h"
#include "semantic/SemanticAnalyzer.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: medlang_lexer archivo.med\n";
        return 1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in) {
        std::cerr << "No se pudo abrir el archivo: " << argv[1] << "\n";
        return 1;
    }

    std::ostringstream buffer;
    buffer << in.rdbuf();
    std::string source = buffer.str();

    Lexer lexer(source);
    std::vector<Token> tokens;

    while (true) {
        Token token = lexer.nextToken();
        tokens.push_back(token);

        std::cout
            << tokenTypeToString(token.type)
            << " lexeme=\"" << token.lexeme << "\""
            << " line=" << token.line
            << " col=" << token.column
            << "\n";

        if (token.type == TokenType::EndOfFile) {
            break;
        }
    }

    const std::vector<LexicalError>& errors = lexer.getErrors();
    if (!errors.empty()) {
        std::cout << "\nTabla de errores lexicos\n";
        std::cout << std::left
                  << std::setw(8) << "No."
                  << std::setw(20) << "Lexema"
                  << std::setw(20) << "Tipo"
                  << std::setw(45) << "Descripcion"
                  << "Posicion\n";

        for (std::size_t i = 0; i < errors.size(); ++i) {
            const LexicalError& error = errors[i];
            std::cout << std::left
                      << std::setw(8) << error.number
                      << std::setw(20) << error.invalidLexeme
                      << std::setw(20) << error.errorType
                      << std::setw(45) << error.description
                      << "(" << error.line << ", " << error.column << ")\n";
        }
    }

    const std::string reportPath = "reporte_lexico.html";
    if (writeLexicalHtmlReport(reportPath, tokens, errors)) {
        std::cout << "\nReporte HTML generado: " << reportPath << "\n";
    } else {
        std::cerr << "\nNo se pudo generar el reporte HTML: " << reportPath << "\n";
    }

    Parser parser(tokens);
    const bool syntaxOk = parser.parseProgram();
    const std::vector<SyntaxError>& syntaxErrors = parser.getErrors();

    if (syntaxErrors.empty()) {
        std::cout << "\nAnalisis sintactico: sin errores en bloques PACIENTES, MEDICOS, CITAS y DIAGNOSTICOS.\n";
    } else {
        std::cout << "\nTabla de errores sintacticos\n";
        std::cout << std::left
                  << std::setw(8) << "No."
                  << std::setw(22) << "Mensaje"
                  << std::setw(22) << "Esperado"
                  << std::setw(35) << "Encontrado"
                  << "Posicion\n";

        for (std::size_t i = 0; i < syntaxErrors.size(); ++i) {
            const SyntaxError& error = syntaxErrors[i];
            std::cout << std::left
                      << std::setw(8) << error.number
                      << std::setw(22) << error.message
                      << std::setw(22) << error.expected
                      << std::setw(35) << error.found
                      << "(" << error.line << ", " << error.column << ")\n";
        }
    }

    if (!syntaxOk) {
        return 2;
    }

    const HospitalData& hospitalData = parser.getHospitalData();
    SemanticAnalyzer semanticAnalyzer(hospitalData);
    const SemanticRecognitionResult recognition = semanticAnalyzer.recognizeInput();

    std::cout << "\nReconocimiento de entrada semantica\n";
    for (std::size_t i = 0; i < recognition.notes.size(); ++i) {
        std::cout << "- " << recognition.notes[i] << "\n";
    }

    if (!recognition.inputReady) {
        std::cout << "Entrada semantica incompleta para reglas avanzadas.\n";
    } else {
        std::cout << "Entrada semantica lista para aplicar reglas.\n";
    }

    const SemanticValidationResult semanticValidation = semanticAnalyzer.validateBasicRules();
    if (semanticValidation.errors.empty()) {
        std::cout << "\nAnalisis semantico base: sin errores.\n";
    } else {
        std::cout << "\nTabla de errores semanticos\n";
        std::cout << std::left
                  << std::setw(8) << "No."
                  << std::setw(22) << "Tipo"
                  << std::setw(50) << "Descripcion"
                  << std::setw(30) << "Entidad"
                  << "Linea\n";

        for (std::size_t i = 0; i < semanticValidation.errors.size(); ++i) {
            const SemanticError& err = semanticValidation.errors[i];
            std::cout << std::left
                      << std::setw(8) << err.number
                      << std::setw(22) << err.type
                      << std::setw(50) << err.description
                      << std::setw(30) << err.entity
                      << err.line << "\n";
        }
    }

    return 0;
}
