#ifndef SYNTAX_ANALYZER_H
#define SYNTAX_ANALYZER_H

#include "../lexer/Token.h"
#include "../lexer/LexicalAnalyzer.h"
#include <string>
#include <vector>

struct SyntaxError {
    int number;
    std::string lexeme;
    std::string errorType;
    std::string description;
    int line;
    int column;
};

struct ASTNode {
    std::string label;
    std::string value;
    int line;
    int column;
    std::vector<ASTNode*> children;
    ~ASTNode() { for (auto c : children) delete c; }
};

class SyntaxAnalyzer {
public:
    explicit SyntaxAnalyzer(LexicalAnalyzer& lexer);
    bool parse(); // returns true if parsed (may contain errors)
    ASTNode* getRoot() const { return root; }
    const std::vector<SyntaxError>& getErrors() const { return m_errors; }

private:
    LexicalAnalyzer& m_lexer;
    Token m_current;
    int m_errorCounter{0};
    std::vector<SyntaxError> m_errors;
    ASTNode* root{nullptr};

    Token advance();
    bool match(TokenType t);
    void consumeExpected(TokenType t, const std::string& errMsg);
    void synchronize();

    // grammar production methods
    ASTNode* parseProgram();
    ASTNode* parseBoard();
    ASTNode* parseColumn();
    ASTNode* parseTask();
};

#endif
