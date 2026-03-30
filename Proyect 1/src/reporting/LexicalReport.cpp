#include "LexicalReport.h"

#include <fstream>

namespace {

std::string htmlEscape(const std::string& input) {
    std::string out;
    out.reserve(input.size());

    for (std::size_t i = 0; i < input.size(); ++i) {
        const char c = input[i];
        switch (c) {
            case '&': out += "&amp;"; break;
            case '<': out += "&lt;"; break;
            case '>': out += "&gt;"; break;
            case '"': out += "&quot;"; break;
            case '\'': out += "&#39;"; break;
            default: out += c; break;
        }
    }

    return out;
}

} // namespace

bool writeLexicalHtmlReport(
    const std::string& outputPath,
    const std::vector<Token>& tokens,
    const std::vector<LexicalError>& errors
) {
    std::ofstream out(outputPath.c_str(), std::ios::out | std::ios::trunc);
    if (!out) {
        return false;
    }

    out << "<!doctype html>\n";
    out << "<html lang=\"es\">\n";
    out << "<head>\n";
    out << "  <meta charset=\"utf-8\">\n";
    out << "  <title>Reporte Lexico MedLang</title>\n";
    out << "  <style>\n";
    out << "    body { font-family: 'Segoe UI', Tahoma, sans-serif; margin: 24px; background: #f5f7fb; color: #1f2937; }\n";
    out << "    h1, h2 { margin: 0 0 12px 0; }\n";
    out << "    .card { background: #ffffff; border: 1px solid #dbe2ea; border-radius: 10px; padding: 16px; margin-bottom: 18px; }\n";
    out << "    .ok { color: #065f46; font-weight: 600; }\n";
    out << "    .warn { color: #b45309; font-weight: 600; }\n";
    out << "    table { width: 100%; border-collapse: collapse; font-size: 14px; }\n";
    out << "    th, td { border: 1px solid #dbe2ea; padding: 8px 10px; text-align: left; }\n";
    out << "    th { background: #eef3ff; }\n";
    out << "    .mono { font-family: Consolas, 'Courier New', monospace; }\n";
    out << "  </style>\n";
    out << "</head>\n";
    out << "<body>\n";

    out << "  <div class=\"card\">\n";
    out << "    <h1>Reporte Lexico - MedLang</h1>\n";
    out << "    <p>Tokens generados: <strong>" << tokens.size() << "</strong></p>\n";
    out << "    <p>Errores lexicos: <strong>" << errors.size() << "</strong></p>\n";
    if (errors.empty()) {
        out << "    <p class=\"ok\">Analisis lexico completado sin errores.</p>\n";
    } else {
        out << "    <p class=\"warn\">Se detectaron errores lexicos. El analisis continuo sin detenerse.</p>\n";
    }
    out << "  </div>\n";

    out << "  <div class=\"card\">\n";
    out << "    <h2>Tabla de Tokens</h2>\n";
    out << "    <table>\n";
    out << "      <thead><tr><th>#</th><th>Token</th><th>Lexema</th><th>Linea</th><th>Columna</th></tr></thead>\n";
    out << "      <tbody>\n";

    for (std::size_t i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens[i];
        out << "        <tr>";
        out << "<td>" << (i + 1) << "</td>";
        out << "<td class=\"mono\">" << htmlEscape(tokenTypeToString(token.type)) << "</td>";
        out << "<td class=\"mono\">" << htmlEscape(token.lexeme) << "</td>";
        out << "<td>" << token.line << "</td>";
        out << "<td>" << token.column << "</td>";
        out << "</tr>\n";
    }

    out << "      </tbody>\n";
    out << "    </table>\n";
    out << "  </div>\n";

    out << "  <div class=\"card\">\n";
    out << "    <h2>Tabla de Errores Lexicos</h2>\n";
    out << "    <table>\n";
    out << "      <thead><tr><th>No.</th><th>Lexema invalido</th><th>Tipo</th><th>Descripcion</th><th>Posicion</th></tr></thead>\n";
    out << "      <tbody>\n";

    if (errors.empty()) {
        out << "        <tr><td colspan=\"5\" class=\"ok\">Sin errores lexicos.</td></tr>\n";
    } else {
        for (std::size_t i = 0; i < errors.size(); ++i) {
            const LexicalError& error = errors[i];
            out << "        <tr>";
            out << "<td>" << error.number << "</td>";
            out << "<td class=\"mono\">" << htmlEscape(error.invalidLexeme) << "</td>";
            out << "<td>" << htmlEscape(error.errorType) << "</td>";
            out << "<td>" << htmlEscape(error.description) << "</td>";
            out << "<td>(" << error.line << ", " << error.column << ")</td>";
            out << "</tr>\n";
        }
    }

    out << "      </tbody>\n";
    out << "    </table>\n";
    out << "  </div>\n";

    out << "</body>\n";
    out << "</html>\n";

    return true;
}
