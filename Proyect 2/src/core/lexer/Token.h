#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    EndOfFile,
    Unknown,

    // Reserved keywords (TaskScript)
    TABLERO,
    COLUMNA,
    TAREA,
    PRIORIDAD,
    RESPONSABLE,
    FECHA_LIMITE,
    
    // Priority enum values
    ALTA,
    MEDIA,
    BAJA,

    // Literals and identifiers
    CADENA,
    ENTERO,
    FECHA_LITERAL,
    IDENTIFICADOR,

    // Structural symbols
    LBrace,      // {
    RBrace,      // }
    LParen,      // (
    RParen,      // )
    Colon,       // :
    Semicolon,   // ;
    Comma,       // ,
    Equal,       // =
    LBracket,    // [
    RBracket,    // ]
    Minus        // -
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;
    int column;
};

inline std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::EndOfFile: return "EndOfFile";
        case TokenType::Unknown: return "Unknown";

        case TokenType::TABLERO: return "TABLERO";
        case TokenType::COLUMNA: return "COLUMNA";
        case TokenType::TAREA: return "TAREA";
        case TokenType::PRIORIDAD: return "PRIORIDAD";
        case TokenType::RESPONSABLE: return "RESPONSABLE";
        case TokenType::FECHA_LIMITE: return "FECHA_LIMITE";

        case TokenType::ALTA: return "ALTA";
        case TokenType::MEDIA: return "MEDIA";
        case TokenType::BAJA: return "BAJA";

        case TokenType::CADENA: return "CADENA";
        case TokenType::ENTERO: return "ENTERO";
        case TokenType::FECHA_LITERAL: return "FECHA_LITERAL";
        case TokenType::IDENTIFICADOR: return "IDENTIFICADOR";

        case TokenType::LBrace: return "LBrace";
        case TokenType::RBrace: return "RBrace";
        case TokenType::LParen: return "LParen";
        case TokenType::RParen: return "RParen";
        case TokenType::Colon: return "Colon";
        case TokenType::Semicolon: return "Semicolon";
        case TokenType::Comma: return "Comma";
        case TokenType::Equal: return "Equal";
        case TokenType::LBracket: return "LBracket";
        case TokenType::RBracket: return "RBracket";
        case TokenType::Minus: return "Minus";
    }

    return "Unknown";
}

#endif
