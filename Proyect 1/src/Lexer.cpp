#include "Lexer.h"

Lexer::Lexer(const std::string& source)
    : m_source(source), m_index(0), m_line(1), m_column(1), m_errorCounter(0) {
    m_keywords["HOSPITAL"] = TokenType::Hospital;
    m_keywords["PACIENTES"] = TokenType::Pacientes;
    m_keywords["paciente"] = TokenType::Paciente;
    m_keywords["MEDICOS"] = TokenType::Medicos;
    m_keywords["medico"] = TokenType::Medico;
    m_keywords["CITAS"] = TokenType::Citas;
    m_keywords["cita"] = TokenType::Cita;
    m_keywords["DIAGNOSTICOS"] = TokenType::Diagnosticos;
    m_keywords["diagnostico"] = TokenType::Diagnostico;
    m_keywords["especialidad"] = TokenType::Especialidad;
    m_keywords["fecha"] = TokenType::Fecha;
    m_keywords["hora"] = TokenType::Hora;
    m_keywords["dosis"] = TokenType::Dosis;
    m_keywords["tipo_sangre"] = TokenType::Tipo_Sangre;
    m_keywords["codigo"] = TokenType::Codigo;
    m_keywords["condicion"] = TokenType::Condicion;
    m_keywords["medicamento"] = TokenType::Medicamento;
    m_keywords["edad"] = TokenType::Edad;
    m_keywords["habitacion"] = TokenType::Habitacion;
    m_keywords["con"] = TokenType::Con;

    m_specialties.insert("CARDIOLOGIA");
    m_specialties.insert("NEUROLOGIA");
    m_specialties.insert("PEDIATRIA");
    m_specialties.insert("CIRUGIA");
    m_specialties.insert("MEDICINA_GENERAL");
    m_specialties.insert("ONCOLOGIA");

    m_doseFrequencies.insert("DIARIA");
    m_doseFrequencies.insert("CADA_8_HORAS");
    m_doseFrequencies.insert("CADA_12_HORAS");
    m_doseFrequencies.insert("SEMANAL");

    m_bloodTypes.insert("A+");
    m_bloodTypes.insert("A-");
    m_bloodTypes.insert("B+");
    m_bloodTypes.insert("B-");
    m_bloodTypes.insert("O+");
    m_bloodTypes.insert("O-");
    m_bloodTypes.insert("AB+");
    m_bloodTypes.insert("AB-");
}

const std::vector<LexicalError>& Lexer::getErrors() const {
    return m_errors;
}

Token Lexer::nextToken() {
    skipWhitespaceAndComments();

    if (isAtEnd()) {
        return makeToken(TokenType::EndOfFile, "", m_line, m_column);
    }

    const int startLine = m_line;
    const int startColumn = m_column;
    const char c = advance();

    switch (c) {
        case '{': return makeToken(TokenType::LBrace, "{", startLine, startColumn);
        case '}': return makeToken(TokenType::RBrace, "}", startLine, startColumn);
        case '(': return makeToken(TokenType::LParen, "(", startLine, startColumn);
        case ')': return makeToken(TokenType::RParen, ")", startLine, startColumn);
        case ':': return makeToken(TokenType::Colon, ":", startLine, startColumn);
        case ';': return makeToken(TokenType::Semicolon, ";", startLine, startColumn);
        case ',': return makeToken(TokenType::Comma, ",", startLine, startColumn);
        case '=': return makeToken(TokenType::Equal, "=", startLine, startColumn);
        case '[': return makeToken(TokenType::LBracket, "[", startLine, startColumn);
        case ']': return makeToken(TokenType::RBracket, "]", startLine, startColumn);
        case '-': return makeToken(TokenType::Minus, "-", startLine, startColumn);
        case '"': {
            // The opening quote was consumed; scan until closing quote.
            return scanString();
        }
        default:
            break;
    }

    if (isAlpha(c)) {
        // Rewind one char so the state machine scans from the first letter.
        m_index--;
        m_column--;
        return scanIdentifierOrKeyword();
    }

    if (isDigit(c)) {
        // Rewind one char so the state machine scans from the first digit.
        m_index--;
        m_column--;
        return scanNumber();
    }

    const std::string invalid(1, c);
    addError(invalid, "CaracterInvalido", "Caracter no reconocido por el lenguaje", startLine, startColumn);
    return makeToken(TokenType::Unknown, invalid, startLine, startColumn);
}

bool Lexer::isAtEnd() const {
    return m_index >= m_source.size();
}

char Lexer::peek() const {
    if (isAtEnd()) {
        return '\0';
    }
    return m_source[m_index];
}

char Lexer::peekNext() const {
    if (m_index + 1 >= m_source.size()) {
        return '\0';
    }
    return m_source[m_index + 1];
}

char Lexer::advance() {
    const char c = m_source[m_index++];

    if (c == '\n') {
        m_line++;
        m_column = 1;
    } else {
        m_column++;
    }

    return c;
}

void Lexer::skipWhitespaceAndComments() {
    while (!isAtEnd()) {
        const char c = peek();

        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance();
            continue;
        }

        // Line comments: // text...
        if (c == '/' && peekNext() == '/') {
            while (!isAtEnd() && peek() != '\n') {
                advance();
            }
            continue;
        }

        break;
    }
}

void Lexer::addError(const std::string& invalidLexeme, const std::string& errorType, const std::string& description, int line, int column) {
    LexicalError error;
    error.number = ++m_errorCounter;
    error.invalidLexeme = invalidLexeme;
    error.errorType = errorType;
    error.description = description;
    error.line = line;
    error.column = column;
    m_errors.push_back(error);
}

Token Lexer::makeToken(TokenType type, const std::string& lexeme, int line, int column) const {
    Token token;
    token.type = type;
    token.lexeme = lexeme;
    token.line = line;
    token.column = column;
    return token;
}

Token Lexer::scanIdentifierOrKeyword() {
    const int startLine = m_line;
    const int startColumn = m_column;
    std::string lexeme;

    while (!isAtEnd() && isAlphaNumeric(peek())) {
        lexeme.push_back(advance());
    }

    const auto it = m_keywords.find(lexeme);
    if (it != m_keywords.end()) {
        return makeToken(it->second, lexeme, startLine, startColumn);
    }

    if (m_specialties.find(lexeme) != m_specialties.end()) {
        return makeToken(TokenType::ESPECIALIDAD_ENUM, lexeme, startLine, startColumn);
    }

    if (m_doseFrequencies.find(lexeme) != m_doseFrequencies.end()) {
        return makeToken(TokenType::DOSIS_ENUM, lexeme, startLine, startColumn);
    }

    return makeToken(TokenType::IDENTIFICADOR, lexeme, startLine, startColumn);
}

Token Lexer::scanNumber() {
    const int startLine = m_line;
    const int startColumn = m_column;
    std::string firstPart;

    while (!isAtEnd() && isDigit(peek())) {
        firstPart.push_back(advance());
    }

    // Fecha AAAA-MM-DD
    if (!isAtEnd() && peek() == '-' && firstPart.size() == 4 && isDigit(peekNext())) {
        std::string lexeme = firstPart;
        lexeme.push_back(advance());

        int count = 0;
        while (!isAtEnd() && isDigit(peek()) && count < 2) {
            lexeme.push_back(advance());
            count++;
        }

        if (count == 2 && !isAtEnd() && peek() == '-' && isDigit(peekNext())) {
            lexeme.push_back(advance());
            count = 0;
            while (!isAtEnd() && isDigit(peek()) && count < 2) {
                lexeme.push_back(advance());
                count++;
            }

            if (count == 2) {
                if (isValidDate(lexeme)) {
                    return makeToken(TokenType::FECHA_LITERAL, lexeme, startLine, startColumn);
                }

                addError(lexeme, "FechaInvalida", "La fecha debe cumplir AAAA-MM-DD con mes 01-12 y dia 01-31", startLine, startColumn);
                return makeToken(TokenType::Unknown, lexeme, startLine, startColumn);
            }
        }

        addError(lexeme, "FechaInvalida", "Formato de fecha incompleto", startLine, startColumn);
        return makeToken(TokenType::Unknown, lexeme, startLine, startColumn);
    }

    // Hora HH:MM
    if (!isAtEnd() && peek() == ':' && firstPart.size() == 2 && isDigit(peekNext())) {
        std::string lexeme = firstPart;
        lexeme.push_back(advance());

        int count = 0;
        while (!isAtEnd() && isDigit(peek()) && count < 2) {
            lexeme.push_back(advance());
            count++;
        }

        if (count == 2) {
            if (isValidHour(lexeme)) {
                return makeToken(TokenType::HORA_LITERAL, lexeme, startLine, startColumn);
            }

            addError(lexeme, "HoraInvalida", "La hora debe cumplir HH:MM en formato 24 horas", startLine, startColumn);
            return makeToken(TokenType::Unknown, lexeme, startLine, startColumn);
        }

        addError(lexeme, "HoraInvalida", "Formato de hora incompleto", startLine, startColumn);
        return makeToken(TokenType::Unknown, lexeme, startLine, startColumn);
    }

    if (firstPart.size() > 1 && firstPart[0] == '0') {
        // Permitimos enteros positivos con ceros a la izquierda por simplicidad en fase lexica.
    }

    return makeToken(TokenType::ENTERO, firstPart, startLine, startColumn);
}

Token Lexer::scanString() {
    const int startLine = m_line;
    const int startColumn = m_column - 1;
    std::string lexeme;

    while (!isAtEnd() && peek() != '"') {
        // Accept new lines inside string; track line/column through advance().
        lexeme.push_back(advance());
    }

    if (isAtEnd()) {
        // Unterminated string: return Unknown token to surface lexical error.
        addError(lexeme, "CadenaNoTerminada", "No se encontro comilla de cierre para la cadena", startLine, startColumn);
        return makeToken(TokenType::Unknown, lexeme, startLine, startColumn);
    }

    // Consume closing quote
    advance();

    if (m_bloodTypes.find(lexeme) != m_bloodTypes.end()) {
        return makeToken(TokenType::TIPO_SANGRE_LITERAL, lexeme, startLine, startColumn);
    }

    if (isValidCodeId(lexeme)) {
        return makeToken(TokenType::CODIGO_ID, lexeme, startLine, startColumn);
    }

    return makeToken(TokenType::CADENA, lexeme, startLine, startColumn);
}

bool Lexer::isValidCodeId(const std::string& text) const {
    if (text.size() < 5) {
        return false;
    }

    if (!(text[0] >= 'A' && text[0] <= 'Z') ||
        !(text[1] >= 'A' && text[1] <= 'Z') ||
        !(text[2] >= 'A' && text[2] <= 'Z') ||
        text[3] != '-') {
        return false;
    }

    for (std::size_t i = 4; i < text.size(); ++i) {
        if (!isDigit(text[i])) {
            return false;
        }
    }

    return true;
}

bool Lexer::isValidDate(const std::string& text) const {
    if (text.size() != 10 || text[4] != '-' || text[7] != '-') {
        return false;
    }

    const int month = (text[5] - '0') * 10 + (text[6] - '0');
    const int day = (text[8] - '0') * 10 + (text[9] - '0');

    if (month < 1 || month > 12) {
        return false;
    }

    if (day < 1 || day > 31) {
        return false;
    }

    return true;
}

bool Lexer::isValidHour(const std::string& text) const {
    if (text.size() != 5 || text[2] != ':') {
        return false;
    }

    const int hour = (text[0] - '0') * 10 + (text[1] - '0');
    const int minute = (text[3] - '0') * 10 + (text[4] - '0');

    if (hour < 0 || hour > 23) {
        return false;
    }

    if (minute < 0 || minute > 59) {
        return false;
    }

    return true;
}

bool Lexer::isAlpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}
