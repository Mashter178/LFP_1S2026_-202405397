#include "Lexer.h"

Lexer::Lexer(const std::string& source)
    : m_source(source), m_index(0), m_line(1), m_column(1), m_errorCounter(0) {
    initializeCatalogs();
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
        case '"': return scanString();
        default: break;
    }

    if (isAlpha(c)) {
        m_index--;
        m_column--;
        return scanIdentifierOrKeyword();
    }

    if (isDigit(c)) {
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

    std::string suggestion;
    if (isLikelyMisspelledReservedWord(lexeme, suggestion)) {
        addError(
            lexeme,
            "PalabraReservadaInvalida",
            "Posible palabra reservada mal escrita. Sugerencia: " + suggestion,
            startLine,
            startColumn
        );
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

    return makeToken(TokenType::ENTERO, firstPart, startLine, startColumn);
}

Token Lexer::scanString() {
    const int startLine = m_line;
    const int startColumn = m_column - 1;
    std::string lexeme;

    while (!isAtEnd() && peek() != '"') {
        lexeme.push_back(advance());
    }

    if (isAtEnd()) {
        addError(lexeme, "CadenaNoTerminada", "No se encontro comilla de cierre para la cadena", startLine, startColumn);
        return makeToken(TokenType::Unknown, lexeme, startLine, startColumn);
    }

    advance();

    if (m_bloodTypes.find(lexeme) != m_bloodTypes.end()) {
        return makeToken(TokenType::TIPO_SANGRE_LITERAL, lexeme, startLine, startColumn);
    }

    if (isBloodTypeLike(lexeme)) {
        addError(
            lexeme,
            "TipoSangreInvalido",
            "Tipo de sangre invalido. Valores permitidos: A+, A-, B+, B-, O+, O-, AB+, AB-",
            startLine,
            startColumn
        );
        return makeToken(TokenType::Unknown, lexeme, startLine, startColumn);
    }

    if (isValidCodeId(lexeme)) {
        return makeToken(TokenType::CODIGO_ID, lexeme, startLine, startColumn);
    }

    if (isCodeIdLike(lexeme)) {
        addError(
            lexeme,
            "CodigoIdInvalido",
            "Codigo ID invalido. Formato esperado: 3 letras mayusculas + '-' + digitos (ej. MED-001)",
            startLine,
            startColumn
        );
        return makeToken(TokenType::Unknown, lexeme, startLine, startColumn);
    }

    return makeToken(TokenType::CADENA, lexeme, startLine, startColumn);
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
