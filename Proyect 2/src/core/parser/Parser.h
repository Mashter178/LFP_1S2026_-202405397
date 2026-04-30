#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

#include "../lexer/Token.h"
#include "../model/HospitalModel.h"

struct SyntaxError {
    int number;
    std::string message;
    std::string expected;
    std::string found;
    int line;
    int column;
};

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);

    bool parseProgram();
    const std::vector<SyntaxError>& getErrors() const;
    const Hospital& getHospitalData() const;

private:
    const std::vector<Token>& m_tokens;
    std::size_t m_current;
    int m_errorCounter;
    std::vector<SyntaxError> m_errors;
    Hospital m_hospitalData;

    bool parsePacientesBlock();
    bool parsePacienteDecl();
    bool parseMedicosBlock();
    bool parseMedicoDecl();
    bool parseCitasBlock();
    bool parseCitaDecl();
    bool parseDiagnosticosBlock();
    bool parseDiagnosticoDecl();

    bool isAtEnd() const;
    const Token& peek() const;
    const Token& previous() const;
    const Token& advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool expect(TokenType type, const std::string& expectedDesc);
    bool consume(TokenType type, const std::string& expectedDesc, Token& consumedToken);

    void addError(const std::string& message, const std::string& expectedDesc, const Token& foundToken);
    void synchronize();
};

#endif
