#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Token.h"

struct LexicalError {
    int number;
    std::string invalidLexeme;
    std::string errorType;
    std::string description;
    int line;
    int column;
};

class Lexer {
public:
    explicit Lexer(const std::string& source);

    Token nextToken();
    const std::vector<LexicalError>& getErrors() const;

private:
    std::string m_source;
    std::size_t m_index;
    int m_line;
    int m_column;

    std::unordered_map<std::string, TokenType> m_keywords;
    std::unordered_set<std::string> m_specialties;
    std::unordered_set<std::string> m_doseFrequencies;
    std::unordered_set<std::string> m_bloodTypes;
    std::vector<LexicalError> m_errors;
    int m_errorCounter;

    bool isAtEnd() const;
    char peek() const;
    char peekNext() const;
    char advance();

    void skipWhitespaceAndComments();
    void addError(const std::string& invalidLexeme, const std::string& errorType, const std::string& description, int line, int column);

    Token makeToken(TokenType type, const std::string& lexeme, int line, int column) const;
    Token scanIdentifierOrKeyword();
    Token scanNumber();
    Token scanString();
    bool isValidCodeId(const std::string& text) const;
    bool isValidDate(const std::string& text) const;
    bool isValidHour(const std::string& text) const;

    static bool isAlpha(char c);
    static bool isDigit(char c);
    static bool isAlphaNumeric(char c);
};

#endif
