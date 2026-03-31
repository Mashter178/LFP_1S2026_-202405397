#include "Parser.h"

Parser::Parser(const std::vector<Token>& tokens)
    : m_tokens(tokens), m_current(0), m_errorCounter(0) {}

bool Parser::parseProgram() {
    bool ok = true;

    ok = expect(TokenType::Hospital, "HOSPITAL") && ok;
    ok = expect(TokenType::LBrace, "{") && ok;

    if (!parsePacientesBlock()) {
        ok = false;
        synchronize();
    }

    if (!parseMedicosBlock()) {
        ok = false;
        synchronize();
    }

    if (!parseCitasBlock()) {
        ok = false;
        synchronize();
    }

    if (!parseDiagnosticosBlock()) {
        ok = false;
        synchronize();
    }

    ok = expect(TokenType::RBrace, "}") && ok;
    ok = expect(TokenType::Semicolon, ";") && ok;

    return ok && m_errors.empty();
}

const std::vector<SyntaxError>& Parser::getErrors() const {
    return m_errors;
}

const HospitalData& Parser::getHospitalData() const {
    return m_hospitalData;
}

bool Parser::parsePacientesBlock() {
    bool ok = true;

    ok = expect(TokenType::Pacientes, "PACIENTES") && ok;
    ok = expect(TokenType::LBrace, "{") && ok;

    while (!isAtEnd() && !check(TokenType::RBrace)) {
        if (!parsePacienteDecl()) {
            ok = false;
            const std::size_t beforeSync = m_current;
            synchronize();

            if (m_current == beforeSync && !isAtEnd()) {
                advance();
            }

            if (check(TokenType::RBrace)) {
                break;
            }
        }
    }

    ok = expect(TokenType::RBrace, "}") && ok;
    ok = expect(TokenType::Semicolon, ";") && ok;

    return ok;
}

bool Parser::parsePacienteDecl() {
    bool ok = true;
    Token nombreToken;
    Token edadToken;
    Token sangreToken;
    Token habitacionToken;
    Token declToken;

    ok = consume(TokenType::Paciente, "paciente", declToken) && ok;
    ok = expect(TokenType::Colon, ":") && ok;
    ok = consume(TokenType::CADENA, "CADENA (nombre)", nombreToken) && ok;

    ok = expect(TokenType::LBracket, "[") && ok;

    ok = expect(TokenType::Edad, "edad") && ok;
    ok = expect(TokenType::Colon, ":") && ok;
    ok = consume(TokenType::ENTERO, "ENTERO", edadToken) && ok;

    ok = expect(TokenType::Comma, ",") && ok;

    ok = expect(TokenType::Tipo_Sangre, "tipo_sangre") && ok;
    ok = expect(TokenType::Colon, ":") && ok;
    ok = consume(TokenType::TIPO_SANGRE_LITERAL, "TIPO_SANGRE_LITERAL", sangreToken) && ok;

    ok = expect(TokenType::Comma, ",") && ok;

    ok = expect(TokenType::Habitacion, "habitacion") && ok;
    ok = expect(TokenType::Colon, ":") && ok;
    ok = consume(TokenType::ENTERO, "ENTERO", habitacionToken) && ok;

    ok = expect(TokenType::RBracket, "]") && ok;
    ok = expect(TokenType::Comma, ",") && ok;

    if (ok) {
        PacienteData paciente;
        paciente.nombre = nombreToken.lexeme;
        paciente.edad = std::stoi(edadToken.lexeme);
        paciente.tipoSangre = sangreToken.lexeme;
        paciente.habitacion = std::stoi(habitacionToken.lexeme);
        paciente.line = declToken.line;
        m_hospitalData.pacientes.push_back(paciente);
    }

    return ok;
}

bool Parser::parseMedicosBlock() {
    bool ok = true;

    ok = expect(TokenType::Medicos, "MEDICOS") && ok;
    ok = expect(TokenType::LBrace, "{") && ok;

    while (!isAtEnd() && !check(TokenType::RBrace)) {
        if (!parseMedicoDecl()) {
            ok = false;
            const std::size_t beforeSync = m_current;
            synchronize();

            if (m_current == beforeSync && !isAtEnd()) {
                advance();
            }

            if (check(TokenType::RBrace)) {
                break;
            }
        }
    }

    ok = expect(TokenType::RBrace, "}") && ok;
    ok = expect(TokenType::Semicolon, ";") && ok;

    return ok;
}

bool Parser::parseMedicoDecl() {
    bool ok = true;
    Token nombreToken;
    Token especialidadToken;
    Token codigoToken;
    Token declToken;

    ok = consume(TokenType::Medico, "medico", declToken) && ok;
    ok = expect(TokenType::Colon, ":") && ok;
    ok = consume(TokenType::CADENA, "CADENA (nombre medico)", nombreToken) && ok;

    ok = expect(TokenType::LBracket, "[") && ok;

    ok = expect(TokenType::Especialidad, "especialidad") && ok;
    ok = expect(TokenType::Colon, ":") && ok;
    ok = consume(TokenType::ESPECIALIDAD_ENUM, "ESPECIALIDAD_ENUM", especialidadToken) && ok;

    ok = expect(TokenType::Comma, ",") && ok;

    ok = expect(TokenType::Codigo, "codigo") && ok;
    ok = expect(TokenType::Colon, ":") && ok;
    ok = consume(TokenType::CODIGO_ID, "CODIGO_ID", codigoToken) && ok;

    ok = expect(TokenType::RBracket, "]") && ok;
    ok = expect(TokenType::Comma, ",") && ok;

    if (ok) {
        MedicoData medico;
        medico.nombre = nombreToken.lexeme;
        medico.especialidad = especialidadToken.lexeme;
        medico.codigo = codigoToken.lexeme;
        medico.line = declToken.line;
        m_hospitalData.medicos.push_back(medico);
    }

    return ok;
}

bool Parser::parseCitasBlock() {
    bool ok = true;

    ok = expect(TokenType::Citas, "CITAS") && ok;
    ok = expect(TokenType::LBrace, "{") && ok;

    while (!isAtEnd() && !check(TokenType::RBrace)) {
        if (!parseCitaDecl()) {
            ok = false;
            const std::size_t beforeSync = m_current;
            synchronize();

            if (m_current == beforeSync && !isAtEnd()) {
                advance();
            }

            if (check(TokenType::RBrace)) {
                break;
            }
        }
    }

    ok = expect(TokenType::RBrace, "}") && ok;
    ok = expect(TokenType::Semicolon, ";") && ok;

    return ok;
}

bool Parser::parseCitaDecl() {
    bool ok = true;
    Token pacienteToken;
    Token medicoToken;
    Token fechaToken;
    Token horaToken;
    Token declToken;

    ok = consume(TokenType::Cita, "cita", declToken) && ok;
    ok = expect(TokenType::Colon, ":") && ok;
    ok = consume(TokenType::CADENA, "CADENA (paciente)", pacienteToken) && ok;
    ok = expect(TokenType::Con, "con") && ok;
    ok = consume(TokenType::CADENA, "CADENA (medico)", medicoToken) && ok;

    ok = expect(TokenType::LBracket, "[") && ok;

    ok = expect(TokenType::Fecha, "fecha") && ok;
    ok = expect(TokenType::Colon, ":") && ok;
    ok = consume(TokenType::FECHA_LITERAL, "FECHA_LITERAL", fechaToken) && ok;

    ok = expect(TokenType::Comma, ",") && ok;

    ok = expect(TokenType::Hora, "hora") && ok;
    ok = expect(TokenType::Colon, ":") && ok;
    ok = consume(TokenType::HORA_LITERAL, "HORA_LITERAL", horaToken) && ok;

    ok = expect(TokenType::RBracket, "]") && ok;
    ok = expect(TokenType::Comma, ",") && ok;

    if (ok) {
        CitaData cita;
        cita.paciente = pacienteToken.lexeme;
        cita.medico = medicoToken.lexeme;
        cita.fecha = fechaToken.lexeme;
        cita.hora = horaToken.lexeme;
        cita.line = declToken.line;
        m_hospitalData.citas.push_back(cita);
    }

    return ok;
}

bool Parser::parseDiagnosticosBlock() {
    bool ok = true;

    ok = expect(TokenType::Diagnosticos, "DIAGNOSTICOS") && ok;
    ok = expect(TokenType::LBrace, "{") && ok;

    while (!isAtEnd() && !check(TokenType::RBrace)) {
        if (!parseDiagnosticoDecl()) {
            ok = false;
            const std::size_t beforeSync = m_current;
            synchronize();

            if (m_current == beforeSync && !isAtEnd()) {
                advance();
            }

            if (check(TokenType::RBrace)) {
                break;
            }
        }
    }

    ok = expect(TokenType::RBrace, "}") && ok;
    ok = expect(TokenType::Semicolon, ";") && ok;

    return ok;
}

bool Parser::parseDiagnosticoDecl() {
    bool ok = true;
    Token pacienteToken;
    Token condicionToken;
    Token medicamentoToken;
    Token dosisToken;
    Token declToken;

    ok = consume(TokenType::Diagnostico, "diagnostico", declToken) && ok;
    ok = expect(TokenType::Colon, ":") && ok;
    ok = consume(TokenType::CADENA, "CADENA (paciente)", pacienteToken) && ok;

    ok = expect(TokenType::LBracket, "[") && ok;

    ok = expect(TokenType::Condicion, "condicion") && ok;
    ok = expect(TokenType::Colon, ":") && ok;
    ok = consume(TokenType::CADENA, "CADENA (condicion)", condicionToken) && ok;

    ok = expect(TokenType::Comma, ",") && ok;

    ok = expect(TokenType::Medicamento, "medicamento") && ok;
    ok = expect(TokenType::Colon, ":") && ok;
    ok = consume(TokenType::CADENA, "CADENA (medicamento)", medicamentoToken) && ok;

    ok = expect(TokenType::Comma, ",") && ok;

    ok = expect(TokenType::Dosis, "dosis") && ok;
    ok = expect(TokenType::Colon, ":") && ok;
    ok = consume(TokenType::DOSIS_ENUM, "DOSIS_ENUM", dosisToken) && ok;

    ok = expect(TokenType::RBracket, "]") && ok;
    ok = expect(TokenType::Comma, ",") && ok;

    if (ok) {
        DiagnosticoData diagnostico;
        diagnostico.paciente = pacienteToken.lexeme;
        diagnostico.condicion = condicionToken.lexeme;
        diagnostico.medicamento = medicamentoToken.lexeme;
        diagnostico.dosis = dosisToken.lexeme;
        diagnostico.line = declToken.line;
        m_hospitalData.diagnosticos.push_back(diagnostico);
    }

    return ok;
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::EndOfFile;
}

const Token& Parser::peek() const {
    return m_tokens[m_current];
}

const Token& Parser::previous() const {
    return m_tokens[m_current - 1];
}

const Token& Parser::advance() {
    if (!isAtEnd()) {
        m_current++;
    }
    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) {
        return false;
    }
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::expect(TokenType type, const std::string& expectedDesc) {
    if (match(type)) {
        return true;
    }

    addError("Token inesperado", expectedDesc, peek());
    return false;
}

bool Parser::consume(TokenType type, const std::string& expectedDesc, Token& consumedToken) {
    if (match(type)) {
        consumedToken = previous();
        return true;
    }

    addError("Token inesperado", expectedDesc, peek());
    return false;
}

void Parser::addError(const std::string& message, const std::string& expectedDesc, const Token& foundToken) {
    SyntaxError error;
    error.number = ++m_errorCounter;
    error.message = message;
    error.expected = expectedDesc;
    error.found = tokenTypeToString(foundToken.type) + " (\"" + foundToken.lexeme + "\")";
    error.line = foundToken.line;
    error.column = foundToken.column;
    m_errors.push_back(error);
}

void Parser::synchronize() {
    while (!isAtEnd()) {
        if (m_current > 0 && previous().type == TokenType::Semicolon) {
            return;
        }

        if (peek().type == TokenType::Paciente ||
            peek().type == TokenType::Pacientes ||
            peek().type == TokenType::Medicos ||
            peek().type == TokenType::Citas ||
            peek().type == TokenType::Diagnosticos ||
            peek().type == TokenType::RBrace) {
            return;
        }

        advance();
    }
}
