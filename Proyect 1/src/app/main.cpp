#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "lexer/Lexer.h"
#include "reporting/LexicalReport.h"

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

    return 0;
}
