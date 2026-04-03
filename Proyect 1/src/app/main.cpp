#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "MedLangService.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: medlang_lexer archivo.med\n";
        return 1;
    }

    MedLangService service;
    const MedLangAnalysisResult result = service.analyzeFile(argv[1]);

    if (!result.sourceLoaded) {
        std::cerr << "No se pudo abrir el archivo: " << argv[1] << "\n";
        return 1;
    }

    for (std::size_t i = 0; i < result.tokens.size(); ++i) {
        const Token& token = result.tokens[i];

        std::cout
            << tokenTypeToString(token.type)
            << " lexeme=\"" << token.lexeme << "\""
            << " line=" << token.line
            << " col=" << token.column
            << "\n";
    }

    if (!result.lexicalErrors.empty()) {
        std::cout << "\nTabla de errores lexicos\n";
        std::cout << std::left
            << std::setw(8) << "No."
            << std::setw(20) << "Lexema"
            << std::setw(20) << "Tipo"
            << std::setw(45) << "Descripcion"
            << "Posicion\n";

        for (std::size_t i = 0; i < result.lexicalErrors.size(); ++i) {
            const LexicalError& error = result.lexicalErrors[i];
            std::cout << std::left
                << std::setw(8) << error.number
                << std::setw(20) << error.invalidLexeme
                << std::setw(20) << error.errorType
                << std::setw(45) << error.description
                << "(" << error.line << ", " << error.column << ")\n";
        }
    }

    const std::string reportPath = "output/reporte_lexico.html";
    if (result.lexicalReportGenerated) {
        std::cout << "\nReporte HTML generado: " << reportPath << "\n";
    } else {
        std::cerr << "\nNo se pudo generar el reporte HTML: " << reportPath << "\n";
    }

    if (result.syntaxErrors.empty()) {
        std::cout << "\nAnalisis sintactico: sin errores en bloques PACIENTES, MEDICOS, CITAS y DIAGNOSTICOS.\n";
    } else {
        std::cout << "\nTabla de errores sintacticos\n";
        std::cout << std::left
            << std::setw(8) << "No."
            << std::setw(22) << "Mensaje"
            << std::setw(22) << "Esperado"
            << std::setw(35) << "Encontrado"
            << "Posicion\n";

        for (std::size_t i = 0; i < result.syntaxErrors.size(); ++i) {
            const SyntaxError& error = result.syntaxErrors[i];
            std::cout << std::left
                << std::setw(8) << error.number
                << std::setw(22) << error.message
                << std::setw(22) << error.expected
                << std::setw(35) << error.found
                << "(" << error.line << ", " << error.column << ")\n";
        }
    }

    if (!result.syntaxOk) {
        return 2;
    }

    std::cout << "\nReconocimiento de entrada semantica\n";
    for (std::size_t i = 0; i < result.semanticRecognition.notes.size(); ++i) {
        std::cout << "- " << result.semanticRecognition.notes[i] << "\n";
    }

    if (!result.semanticRecognition.inputReady) {
        std::cout << "Entrada semantica incompleta para reglas avanzadas.\n";
    } else {
        std::cout << "Entrada semantica lista para aplicar reglas.\n";
    }

    if (result.semanticValidation.errors.empty()) {
        std::cout << "\nAnalisis semantico base: sin errores.\n";
    } else {
        std::cout << "\nTabla de errores semanticos\n";
        std::cout << std::left
            << std::setw(8) << "No."
            << std::setw(22) << "Tipo"
            << std::setw(50) << "Descripcion"
            << std::setw(30) << "Entidad"
            << "Linea\n";

        for (std::size_t i = 0; i < result.semanticValidation.errors.size(); ++i) {
            const SemanticError& err = result.semanticValidation.errors[i];
            std::cout << std::left
                << std::setw(8) << err.number
                << std::setw(22) << err.type
                << std::setw(50) << err.description
                << std::setw(30) << err.entity
                << err.line << "\n";
        }
    }

    const std::string reportsDirectory = "output";
    if (result.htmlReportsGenerated) {
        std::cout << "\nReportes HTML generados en: " << reportsDirectory << "\n";
    } else {
        std::cerr << "\nNo se pudieron generar uno o mas reportes HTML en: " << reportsDirectory << "\n";
    }

    return 0;
}
