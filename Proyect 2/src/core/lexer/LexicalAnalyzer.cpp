#include "LexicalAnalyzer.h"
#include <cctype>

namespace {
bool isAllowedStringChar(unsigned char c) {
    if (c >= 128) {
        return true;
    }

    if (std::isalnum(c) || c == ' ' || c == '-' || c == '_' || c == '.' || c == '/') {
        return true;
    }

    return false;
}

void addLexicalError(std::vector<LexicalError>& errors, int& counter, const std::string& lexeme, const std::string& description, int line, int column) {
    LexicalError err;
    err.number = ++counter;
    err.invalidLexeme = lexeme;
    err.errorType = "CaracterNoPermitido";
    err.description = description;
    err.line = line;
    err.column = column;
    errors.push_back(err);
}
}

LexicalAnalyzer::LexicalAnalyzer(const std::string& source)
    : m_source(source), m_index(0), m_line(1), m_column(1) {
    initializeKeywords();
}

void LexicalAnalyzer::reset() {
    m_index = 0; m_line = 1; m_column = 1; m_tokens.clear(); m_errors.clear(); m_errorCounter = 0;
    initializeKeywords();
}

void LexicalAnalyzer::initializeKeywords() {
    m_keywords["TABLERO"] = TokenType::TABLERO;
    m_keywords["COLUMNA"] = TokenType::COLUMNA;
    m_keywords["TAREA"] = TokenType::TAREA;
    m_keywords["PRIORIDAD"] = TokenType::PRIORIDAD;
    m_keywords["RESPONSABLE"] = TokenType::RESPONSABLE;
    m_keywords["FECHA_LIMITE"] = TokenType::FECHA_LIMITE;

    m_keywords["ALTA"] = TokenType::ALTA;
    m_keywords["MEDIA"] = TokenType::MEDIA;
    m_keywords["BAJA"] = TokenType::BAJA;
}

bool LexicalAnalyzer::isAtEnd() const {
    return m_index >= m_source.size();
}

char LexicalAnalyzer::peek() const {
    if (isAtEnd()) return '\0';
    return m_source[m_index];
}

char LexicalAnalyzer::peekNext() const {
    if (m_index + 1 >= m_source.size()) return '\0';
    return m_source[m_index + 1];
}

char LexicalAnalyzer::advance() {
    char c = m_source[m_index++];
    if (c == '\n') { m_line++; m_column = 1; }
    else { m_column++; }
    return c;
}

void LexicalAnalyzer::skipWhitespaceAndComments() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') { advance(); continue; }
        if (c == '/' && peekNext() == '/') { // line comment
            while (!isAtEnd() && peek() != '\n') advance();
            continue;
        }
        if (c == '#') { // shell-style comment
            while (!isAtEnd() && peek() != '\n') advance();
            continue;
        }
        break;
    }
}

bool LexicalAnalyzer::isAlpha(char c) const {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_';
}

bool LexicalAnalyzer::isDigit(char c) const { return c >= '0' && c <= '9'; }
bool LexicalAnalyzer::isAlphaNumeric(char c) const { return isAlpha(c) || isDigit(c); }

Token LexicalAnalyzer::nextToken() {
    skipWhitespaceAndComments();
    if (isAtEnd()) { Token eofTok{TokenType::EndOfFile, "", m_line, m_column}; m_tokens.push_back(eofTok); return eofTok; }

    int startLine = m_line;
    int startColumn = m_column;
    char c = advance();

    switch (c) {
        case '{': { Token tok{TokenType::LBrace, "{", startLine, startColumn}; m_tokens.push_back(tok); return tok; }
        case '}': { Token tok{TokenType::RBrace, "}", startLine, startColumn}; m_tokens.push_back(tok); return tok; }
        case '(': { Token tok{TokenType::LParen, "(", startLine, startColumn}; m_tokens.push_back(tok); return tok; }
        case ')': { Token tok{TokenType::RParen, ")", startLine, startColumn}; m_tokens.push_back(tok); return tok; }
        case ':': { Token tok{TokenType::Colon, ":", startLine, startColumn}; m_tokens.push_back(tok); return tok; }
        case ';': { Token tok{TokenType::Semicolon, ";", startLine, startColumn}; m_tokens.push_back(tok); return tok; }
        case ',': { Token tok{TokenType::Comma, ",", startLine, startColumn}; m_tokens.push_back(tok); return tok; }
        case '=': { Token tok{TokenType::Equal, "=", startLine, startColumn}; m_tokens.push_back(tok); return tok; }
        case '[': { Token tok{TokenType::LBracket, "[", startLine, startColumn}; m_tokens.push_back(tok); return tok; }
        case ']': { Token tok{TokenType::RBracket, "]", startLine, startColumn}; m_tokens.push_back(tok); return tok; }
        case '-': { Token tok{TokenType::Minus, "-", startLine, startColumn}; m_tokens.push_back(tok); return tok; }
        case '"': return scanString(startLine, startColumn);
        default: break;
    }

    if (isAlpha(c)) {
        // roll back one char and scan identifier/keyword
        m_index--; m_column--;
        return scanIdentifierOrKeyword(startLine, startColumn);
    }

    if (isDigit(c)) {
        m_index--; m_column--;
        return scanNumber(startLine, startColumn);
    }

    // Unknown single character
    std::string s(1, c);
    Token tok = Token{TokenType::Unknown, s, startLine, startColumn};
    // record lexical error
    LexicalError err;
    err.number = ++m_errorCounter;
    err.invalidLexeme = tok.lexeme;
    err.errorType = "CaracterInvalido";
    err.description = "Caracter no reconocido por el lenguaje";
    err.line = tok.line; err.column = tok.column;
    m_errors.push_back(err);
    m_tokens.push_back(tok);
    return tok;
}

Token LexicalAnalyzer::scanIdentifierOrKeyword(int startLine, int startColumn) {
    std::string lexeme;
    while (!isAtEnd() && isAlphaNumeric(peek())) lexeme.push_back(advance());
    // normalize to upper for keyword matching
    std::string upper;
    upper.reserve(lexeme.size());
    for (char ch : lexeme) upper.push_back(std::toupper(static_cast<unsigned char>(ch)));

    auto it = m_keywords.find(upper);
    Token tok;
    if (it != m_keywords.end()) {
        tok = Token{it->second, lexeme, startLine, startColumn};
    } else {
        tok = Token{TokenType::IDENTIFICADOR, lexeme, startLine, startColumn};
    }
    m_tokens.push_back(tok);
    return tok;
}

Token LexicalAnalyzer::scanNumber(int startLine, int startColumn) {
    std::string lexeme;
    while (!isAtEnd() && isDigit(peek())) lexeme.push_back(advance());

    // detect date pattern YYYY-MM-DD
    if (!isAtEnd() && peek() == '-' && lexeme.size() == 4 && isDigit(peekNext())) {
        lexeme.push_back(advance()); // '-'
        int count = 0;
        while (!isAtEnd() && isDigit(peek()) && count < 2) { lexeme.push_back(advance()); count++; }
        if (count == 2 && peek() == '-' ) {
            lexeme.push_back(advance()); // '-'
            count = 0;
            while (!isAtEnd() && isDigit(peek()) && count < 2) { lexeme.push_back(advance()); count++; }
            if (count == 2 && isValidDate(lexeme)) {
                Token tok{TokenType::FECHA_LITERAL, lexeme, startLine, startColumn};
                m_tokens.push_back(tok);
                return tok;
            }
        }
        // if not valid date, fall through and return ENTERO for the initial number
    }

    Token tok = Token{TokenType::ENTERO, lexeme, startLine, startColumn};
    m_tokens.push_back(tok);
    return tok;
}

Token LexicalAnalyzer::scanString(int startLine, int startColumn) {
    std::string lexeme;
    while (!isAtEnd() && peek() != '"') {
        // allow escaped quotes simple handling
        if (peek() == '\\' && peekNext() == '"') { advance(); lexeme.push_back(advance()); continue; }

        const unsigned char current = static_cast<unsigned char>(peek());
        if (!isAllowedStringChar(current)) {
            addLexicalError(m_errors, m_errorCounter, std::string(1, static_cast<char>(current)), "Caracter no permitido dentro de cadena", m_line, m_column);
        }

        lexeme.push_back(advance());
    }
    if (isAtEnd()) {
        addLexicalError(m_errors, m_errorCounter, lexeme, "Cadena sin cerrar", startLine, startColumn);
        return Token{TokenType::Unknown, lexeme, startLine, startColumn};
    }
    // consume closing quote
    advance();
    Token tok = Token{TokenType::CADENA, lexeme, startLine, startColumn};
    m_tokens.push_back(tok);
    return tok;
}

bool LexicalAnalyzer::isValidDate(const std::string& s) const {
    // Expect format YYYY-MM-DD (10 chars)
    if (s.size() != 10) return false;
    for (size_t i = 0; i < s.size(); ++i) {
        if (i == 4 || i == 7) { if (s[i] != '-') return false; }
        else if (!isDigit(s[i])) return false;
    }
    int year = std::stoi(s.substr(0,4));
    int month = std::stoi(s.substr(5,2));
    int day = std::stoi(s.substr(8,2));
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    // basic month-day validation
    const int mdays[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int maxd = mdays[month];
    // simple leap year for feb
    if (month == 2) {
        bool leap = (year%4==0 && (year%100!=0 || year%400==0));
        if (leap) maxd = 29;
    }
    return day <= maxd;
}
