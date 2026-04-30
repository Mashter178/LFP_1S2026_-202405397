#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    EndOfFile,
    Unknown,

    // Literales
    CADENA,
    ENTERO,
    FECHA_LITERAL,
    HORA_LITERAL,
    CODIGO_ID,
    TIPO_SANGRE_LITERAL,
    IDENTIFICADOR,

    // Palabras Claves MedLang
    Hospital,
    Pacientes,
    Paciente,
    Medicos,
    Medico,
    Citas,
    Cita,
    Diagnosticos,
    Diagnostico,
    Especialidad,
    Fecha,
    Hora,
    Dosis,
    Tipo_Sangre,
    Codigo,
    Condicion,
    Medicamento,
    Edad,
    Habitacion,
    Con,

    // Enumeraciones
    ESPECIALIDAD_ENUM,
    DOSIS_ENUM,

    // Simbolos
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

        case TokenType::CADENA: return "CADENA";
        case TokenType::ENTERO: return "ENTERO";
        case TokenType::FECHA_LITERAL: return "FECHA_LITERAL";
        case TokenType::HORA_LITERAL: return "HORA_LITERAL";
        case TokenType::CODIGO_ID: return "CODIGO_ID";
        case TokenType::TIPO_SANGRE_LITERAL: return "TIPO_SANGRE_LITERAL";
        case TokenType::IDENTIFICADOR: return "IDENTIFICADOR";

        case TokenType::Hospital: return "Hospital";
        case TokenType::Pacientes: return "Pacientes";
        case TokenType::Paciente: return "Paciente";
        case TokenType::Medicos: return "Medicos";
        case TokenType::Medico: return "Medico";
        case TokenType::Citas: return "Citas";
        case TokenType::Cita: return "Cita";
        case TokenType::Diagnosticos: return "Diagnosticos";
        case TokenType::Diagnostico: return "Diagnostico";
        case TokenType::Especialidad: return "Especialidad";
        case TokenType::Fecha: return "Fecha";
        case TokenType::Hora: return "Hora";
        case TokenType::Dosis: return "Dosis";
        case TokenType::Tipo_Sangre: return "Tipo_Sangre";
        case TokenType::Codigo: return "Codigo";
        case TokenType::Condicion: return "Condicion";
        case TokenType::Medicamento: return "Medicamento";
        case TokenType::Edad: return "Edad";
        case TokenType::Habitacion: return "Habitacion";
        case TokenType::Con: return "Con";

        case TokenType::ESPECIALIDAD_ENUM: return "ESPECIALIDAD_ENUM";
        case TokenType::DOSIS_ENUM: return "DOSIS_ENUM";

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
