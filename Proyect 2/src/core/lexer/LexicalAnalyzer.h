#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include "Token.h"
#include <string>
#include <unordered_map>
#include <vector>

struct LexicalError {
    int number;
    std::string invalidLexeme;
    std::string errorType;
    std::string description;
    int line;
    int column;
};

class LexicalAnalyzer {
public:
    explicit LexicalAnalyzer(const std::string& source);
    Token nextToken();
    const std::vector<Token>& getTokens() const { return m_tokens; }
    const std::vector<LexicalError>& getErrors() const { return m_errors; }

    void reset();

private:
    std::string m_source;
    size_t m_index;
    int m_line;
    int m_column;

    bool isAtEnd() const;
    char peek() const;
    char peekNext() const;
    char advance();
    void skipWhitespaceAndComments();

    bool isAlpha(char c) const;
    bool isDigit(char c) const;
    bool isAlphaNumeric(char c) const;

    Token scanIdentifierOrKeyword(int startLine, int startColumn);
    Token scanNumber(int startLine, int startColumn);
    Token scanString(int startLine, int startColumn);
    bool isValidDate(const std::string& s) const;

    std::unordered_map<std::string, TokenType> m_keywords;
    void initializeKeywords();
    std::vector<Token> m_tokens;
    std::vector<LexicalError> m_errors;
    int m_errorCounter{0};
};

#endif
