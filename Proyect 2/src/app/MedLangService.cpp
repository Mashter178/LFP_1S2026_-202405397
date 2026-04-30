#include "MedLangService.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <cctype>

#include "../core/report/Generator.h"
#include "../core/report/Html.h"
#include "../core/report/LexicalReport.h"

namespace {
std::size_t countHospitalEntities(const Hospital& hospital) {
    return hospital.pacientes.size()
        + hospital.medicos.size()
        + hospital.citas.size()
        + hospital.diagnosticos.size();
}

std::vector<Token> buildRecoveryTokens(const std::vector<Token>& tokens) {
    std::vector<Token> filtered;
    filtered.reserve(tokens.size());

    for (std::size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].type != TokenType::Unknown) {
            filtered.push_back(tokens[i]);
        }
    }

    if (filtered.empty() || filtered.back().type != TokenType::EndOfFile) {
        Token eof;
        eof.type = TokenType::EndOfFile;
        eof.lexeme = "";
        eof.line = filtered.empty() ? 1 : filtered.back().line;
        eof.column = filtered.empty() ? 1 : filtered.back().column;
        filtered.push_back(eof);
    }

    return filtered;
}

template <typename T>
bool findNextTokenValue(
    const std::vector<Token>& tokens,
    std::size_t start,
    std::size_t end,
    TokenType type,
    T& outValue
) {
    const std::size_t upper = end > tokens.size() ? tokens.size() : end;
    for (std::size_t i = start; i < upper; ++i) {
        if (tokens[i].type == type) {
            if constexpr (std::is_same<T, std::string>::value) {
                outValue = tokens[i].lexeme;
            } else {
                try {
                    outValue = static_cast<T>(std::stoi(tokens[i].lexeme));
                } catch (...) {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

Hospital recoverHospitalLoosely(const std::vector<Token>& tokens) {
    Hospital recovered;

    std::unordered_set<std::string> pacienteKeys;
    std::unordered_set<std::string> medicoKeys;
    std::unordered_set<std::string> citaKeys;
    std::unordered_set<std::string> diagnosticoKeys;

    for (std::size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].type == TokenType::Paciente) {
            const std::size_t end = (i + 50 < tokens.size()) ? (i + 50) : tokens.size();

            std::string nombre;
            int edad = 0;
            std::string tipoSangre;
            int habitacion = 0;

            const bool hasNombre = findNextTokenValue(tokens, i + 1, end, TokenType::CADENA, nombre);
            const bool hasEdad = findNextTokenValue(tokens, i + 1, end, TokenType::ENTERO, edad);
            const bool hasTipoSangre = findNextTokenValue(tokens, i + 1, end, TokenType::TIPO_SANGRE_LITERAL, tipoSangre);

            int firstInt = 0;
            const bool hasHabitacion = findNextTokenValue(tokens, i + 2, end, TokenType::ENTERO, firstInt);
            if (hasHabitacion) {
                habitacion = firstInt;
            }

            if (hasNombre && hasEdad) {
                Paciente p;
                p.nombre = nombre;
                p.edad = edad;
                p.tipoSangre = hasTipoSangre ? tipoSangre : "N/A";
                p.habitacion = hasHabitacion ? habitacion : 0;
                p.line = tokens[i].line;

                const std::string key = p.nombre + "|" + std::to_string(p.line);
                if (pacienteKeys.insert(key).second) {
                    recovered.pacientes.push_back(p);
                }
            }
        }

        if (tokens[i].type == TokenType::Medico) {
            const std::size_t end = (i + 40 < tokens.size()) ? (i + 40) : tokens.size();

            std::string nombre;
            std::string especialidad;
            std::string codigo;

            const bool hasNombre = findNextTokenValue(tokens, i + 1, end, TokenType::CADENA, nombre);
            const bool hasEspecialidad = findNextTokenValue(tokens, i + 1, end, TokenType::ESPECIALIDAD_ENUM, especialidad);
            const bool hasCodigo = findNextTokenValue(tokens, i + 1, end, TokenType::CODIGO_ID, codigo);

            if (hasNombre && (hasEspecialidad || hasCodigo)) {
                Medico m;
                m.nombre = nombre;
                m.especialidad = hasEspecialidad ? especialidad : "NO_DEFINIDA";
                m.codigo = hasCodigo ? codigo : "NO_CODIGO";
                m.line = tokens[i].line;

                const std::string key = m.nombre + "|" + std::to_string(m.line);
                if (medicoKeys.insert(key).second) {
                    recovered.medicos.push_back(m);
                }
            }
        }

        if (tokens[i].type == TokenType::Cita) {
            const std::size_t end = (i + 50 < tokens.size()) ? (i + 50) : tokens.size();

            std::string paciente;
            std::string medico;
            std::string fecha;
            std::string hora;

            bool hasPaciente = false;
            bool hasMedico = false;
            for (std::size_t j = i + 1; j < end; ++j) {
                if (!hasPaciente && tokens[j].type == TokenType::CADENA) {
                    paciente = tokens[j].lexeme;
                    hasPaciente = true;
                    continue;
                }
                if (hasPaciente && !hasMedico && tokens[j].type == TokenType::CADENA) {
                    medico = tokens[j].lexeme;
                    hasMedico = true;
                    break;
                }
            }

            const bool hasFecha = findNextTokenValue(tokens, i + 1, end, TokenType::FECHA_LITERAL, fecha);
            const bool hasHora = findNextTokenValue(tokens, i + 1, end, TokenType::HORA_LITERAL, hora);

            if (hasPaciente && hasMedico && (hasFecha || hasHora)) {
                Cita c;
                c.paciente = paciente;
                c.medico = medico;
                c.fecha = hasFecha ? fecha : "0000-00-00";
                c.hora = hasHora ? hora : "00:00";
                c.line = tokens[i].line;

                const std::string key = c.paciente + "|" + c.medico + "|" + std::to_string(c.line);
                if (citaKeys.insert(key).second) {
                    recovered.citas.push_back(c);
                }
            }
        }

        if (tokens[i].type == TokenType::Diagnostico) {
            const std::size_t end = (i + 60 < tokens.size()) ? (i + 60) : tokens.size();

            std::string paciente;
            std::string condicion;
            std::string medicamento;
            std::string dosis;

            bool hasPaciente = false;
            bool hasCondicion = false;
            bool hasMedicamento = false;
            for (std::size_t j = i + 1; j < end; ++j) {
                if (tokens[j].type != TokenType::CADENA) {
                    continue;
                }

                if (!hasPaciente) {
                    paciente = tokens[j].lexeme;
                    hasPaciente = true;
                    continue;
                }
                if (!hasCondicion) {
                    condicion = tokens[j].lexeme;
                    hasCondicion = true;
                    continue;
                }
                if (!hasMedicamento) {
                    medicamento = tokens[j].lexeme;
                    hasMedicamento = true;
                    break;
                }
            }

            const bool hasDosis = findNextTokenValue(tokens, i + 1, end, TokenType::DOSIS_ENUM, dosis);

            if (hasPaciente && (hasCondicion || hasMedicamento)) {
                Diagnostico d;
                d.paciente = paciente;
                d.condicion = hasCondicion ? condicion : "SIN_CONDICION";
                d.medicamento = hasMedicamento ? medicamento : "SIN_MEDICAMENTO";
                d.dosis = hasDosis ? dosis : "N/A";
                d.line = tokens[i].line;

                const std::string key = d.paciente + "|" + std::to_string(d.line);
                if (diagnosticoKeys.insert(key).second) {
                    recovered.diagnosticos.push_back(d);
                }
            }
        }
    }

    return recovered;
}

std::string trim(const std::string& value) {
    std::size_t begin = 0;
    while (begin < value.size() && std::isspace(static_cast<unsigned char>(value[begin]))) {
        begin++;
    }

    std::size_t end = value.size();
    while (end > begin && std::isspace(static_cast<unsigned char>(value[end - 1]))) {
        end--;
    }

    return value.substr(begin, end - begin);
}

std::string stripWrappers(std::string value) {
    value = trim(value);

    if (!value.empty() && (value.front() == '"' || value.front() == '\'')) {
        value.erase(value.begin());
    }

    while (!value.empty() && (value.back() == ',' || value.back() == ']' || value.back() == '"' || value.back() == '\'')) {
        value.pop_back();
    }

    return trim(value);
}

std::string extractAfterLabel(const std::string& line, const std::string& label) {
    const std::size_t labelPos = line.find(label);
    if (labelPos == std::string::npos) {
        return "";
    }

    std::size_t valueStart = labelPos + label.size();
    while (valueStart < line.size() && std::isspace(static_cast<unsigned char>(line[valueStart]))) {
        valueStart++;
    }

    std::size_t valueEnd = valueStart;
    while (valueEnd < line.size() && line[valueEnd] != ',' && line[valueEnd] != ']') {
        valueEnd++;
    }

    return stripWrappers(line.substr(valueStart, valueEnd - valueStart));
}

std::string extractNameBeforeMarker(const std::string& line, const std::string& prefix, const std::string& marker) {
    const std::size_t start = line.find(prefix);
    if (start == std::string::npos) {
        return "";
    }

    std::size_t valueStart = start + prefix.size();
    while (valueStart < line.size() && std::isspace(static_cast<unsigned char>(line[valueStart]))) {
        valueStart++;
    }

    std::size_t valueEnd = line.find(marker, valueStart);
    if (valueEnd == std::string::npos) {
        valueEnd = line.find('[', valueStart);
        if (valueEnd == std::string::npos) {
            valueEnd = line.size();
        }
    }

    return stripWrappers(line.substr(valueStart, valueEnd - valueStart));
}

int parseIntOrDefault(const std::string& text, int fallback) {
    try {
        return std::stoi(text);
    } catch (...) {
        return fallback;
    }
}

Hospital recoverHospitalFromSourceLines(const std::string& source) {
    Hospital recovered;

    std::unordered_set<std::string> pacienteKeys;
    std::unordered_set<std::string> medicoKeys;
    std::unordered_set<std::string> citaKeys;
    std::unordered_set<std::string> diagnosticoKeys;

    std::istringstream input(source);
    std::string line;
    int lineNumber = 0;

    while (std::getline(input, line)) {
        lineNumber++;
        const std::string clean = trim(line);

        if (clean.find("paciente:") != std::string::npos) {
            Paciente p;
            p.nombre = extractNameBeforeMarker(clean, "paciente:", "[edad:");
            p.edad = parseIntOrDefault(extractAfterLabel(clean, "edad:"), 0);
            p.tipoSangre = extractAfterLabel(clean, "tipo_sangre:");
            p.habitacion = parseIntOrDefault(extractAfterLabel(clean, "habitacion:"), 0);
            p.line = lineNumber;

            if (!p.nombre.empty()) {
                const std::string key = p.nombre + "|" + std::to_string(p.line);
                if (pacienteKeys.insert(key).second) {
                    if (p.tipoSangre.empty()) {
                        p.tipoSangre = "N/A";
                    }
                    recovered.pacientes.push_back(p);
                }
            }
        }

        if (clean.find("medico:") != std::string::npos) {
            Medico m;
            m.nombre = extractNameBeforeMarker(clean, "medico:", "[especialidad:");
            m.especialidad = extractAfterLabel(clean, "especialidad:");
            m.codigo = extractAfterLabel(clean, "codigo:");
            m.line = lineNumber;

            if (!m.nombre.empty()) {
                const std::string key = m.nombre + "|" + std::to_string(m.line);
                if (medicoKeys.insert(key).second) {
                    if (m.especialidad.empty()) {
                        m.especialidad = "NO_DEFINIDA";
                    }
                    if (m.codigo.empty()) {
                        m.codigo = "NO_CODIGO";
                    }
                    recovered.medicos.push_back(m);
                }
            }
        }

        if (clean.find("cita:") != std::string::npos) {
            Cita c;
            c.paciente = extractNameBeforeMarker(clean, "cita:", " con ");
            c.medico = extractNameBeforeMarker(clean, " con ", " [fecha:");
            c.fecha = extractAfterLabel(clean, "fecha:");
            c.hora = extractAfterLabel(clean, "hora:");
            c.line = lineNumber;

            if (!c.paciente.empty() && !c.medico.empty()) {
                const std::string key = c.paciente + "|" + c.medico + "|" + std::to_string(c.line);
                if (citaKeys.insert(key).second) {
                    if (c.fecha.empty()) {
                        c.fecha = "0000-00-00";
                    }
                    if (c.hora.empty()) {
                        c.hora = "00:00";
                    }
                    recovered.citas.push_back(c);
                }
            }
        }

        if (clean.find("diagnostico:") != std::string::npos) {
            Diagnostico d;
            d.paciente = extractNameBeforeMarker(clean, "diagnostico:", " [condicion:");
            d.condicion = extractAfterLabel(clean, "condicion:");
            d.medicamento = extractAfterLabel(clean, "medicamento:");
            d.dosis = extractAfterLabel(clean, "dosis:");
            d.line = lineNumber;

            if (!d.paciente.empty()) {
                const std::string key = d.paciente + "|" + std::to_string(d.line);
                if (diagnosticoKeys.insert(key).second) {
                    if (d.condicion.empty()) {
                        d.condicion = "SIN_CONDICION";
                    }
                    if (d.medicamento.empty()) {
                        d.medicamento = "SIN_MEDICAMENTO";
                    }
                    if (d.dosis.empty()) {
                        d.dosis = "N/A";
                    }
                    recovered.diagnosticos.push_back(d);
                }
            }
        }
    }

    return recovered;
}

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

MedLangAnalysisResult MedLangService::analyzeFile(const std::string& filePath, bool forceGenerateReports) const {
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

    if (!result.syntaxOk && forceGenerateReports) {
        const std::vector<Token> recoveryTokens = buildRecoveryTokens(result.tokens);
        Parser recoveryParser(recoveryTokens);
        recoveryParser.parseProgram();

        const Hospital recoveredHospital = recoveryParser.getHospitalData();
        if (countHospitalEntities(recoveredHospital) > countHospitalEntities(result.hospital)) {
            result.hospital = recoveredHospital;
        }

        const Hospital looseRecoveryHospital = recoverHospitalLoosely(result.tokens);
        if (looseRecoveryHospital.pacientes.size() > result.hospital.pacientes.size()) {
            result.hospital.pacientes = looseRecoveryHospital.pacientes;
        }
        if (looseRecoveryHospital.medicos.size() > result.hospital.medicos.size()) {
            result.hospital.medicos = looseRecoveryHospital.medicos;
        }
        if (looseRecoveryHospital.citas.size() > result.hospital.citas.size()) {
            result.hospital.citas = looseRecoveryHospital.citas;
        }
        if (looseRecoveryHospital.diagnosticos.size() > result.hospital.diagnosticos.size()) {
            result.hospital.diagnosticos = looseRecoveryHospital.diagnosticos;
        }

        const Hospital sourceRecoveryHospital = recoverHospitalFromSourceLines(source);
        if (sourceRecoveryHospital.pacientes.size() > result.hospital.pacientes.size()) {
            result.hospital.pacientes = sourceRecoveryHospital.pacientes;
        }
        if (sourceRecoveryHospital.medicos.size() > result.hospital.medicos.size()) {
            result.hospital.medicos = sourceRecoveryHospital.medicos;
        }
        if (sourceRecoveryHospital.citas.size() > result.hospital.citas.size()) {
            result.hospital.citas = sourceRecoveryHospital.citas;
        }
        if (sourceRecoveryHospital.diagnosticos.size() > result.hospital.diagnosticos.size()) {
            result.hospital.diagnosticos = sourceRecoveryHospital.diagnosticos;
        }
    }

    if (!result.syntaxOk && !forceGenerateReports) {
        result.htmlReportsGenerated = writeFallbackIndex(result);
        return result;
    }

    SemanticAnalyzer semanticAnalyzer(result.hospital);
    result.semanticRecognition = semanticAnalyzer.recognizeInput();
    result.semanticValidation = semanticAnalyzer.validateBasicRules();

    result.htmlReportsGenerated = Generator::generateAllHtmlReports("output", result.hospital, result.semanticValidation, filePath);
    if (!result.htmlReportsGenerated) {
        if (forceGenerateReports) {
            const std::filesystem::path forcedIndexPath("output/indice_reportes.html");
            result.htmlReportsGenerated = std::filesystem::exists(forcedIndexPath);
        } else {
            result.htmlReportsGenerated = writeFallbackIndex(result);
        }
    }
    return result;
}
