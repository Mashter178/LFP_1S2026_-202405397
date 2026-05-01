#include "SyntaxAnalyzer.h"
#include <iostream>

SyntaxAnalyzer::SyntaxAnalyzer(LexicalAnalyzer& lexer)
    : m_lexer(lexer) {
    m_current = m_lexer.nextToken();
}

Token SyntaxAnalyzer::advance() {
    Token prev = m_current;
    m_current = m_lexer.nextToken();
    return prev;
}

bool SyntaxAnalyzer::match(TokenType t) {
    if (m_current.type == t) { advance(); return true; }
    return false;
}

void SyntaxAnalyzer::consumeExpected(TokenType t, const std::string& errMsg) {
    if (m_current.type == t) { advance(); return; }
    SyntaxError e;
    e.number = ++m_errorCounter;
    e.lexeme = m_current.lexeme;
    e.errorType = "Sintactico";
    e.description = errMsg;
    e.line = m_current.line;
    e.column = m_current.column;
    m_errors.push_back(e);
}

void SyntaxAnalyzer::synchronize() {
    // simple recovery: skip until one of these tokens
    while (m_current.type != TokenType::EndOfFile) {
        if (m_current.type == TokenType::RBrace || m_current.type == TokenType::TABLERO ||
            m_current.type == TokenType::COLUMNA || m_current.type == TokenType::TAREA) return;
        advance();
    }
}

bool SyntaxAnalyzer::parse() {
    if (root) { delete root; root = nullptr; }
    root = parseProgram();
    return true;
}

ASTNode* SyntaxAnalyzer::parseProgram() {
    ASTNode* program = new ASTNode{ "Program", "", 0, 0, {} };
    while (m_current.type != TokenType::EndOfFile) {
        if (m_current.type == TokenType::TABLERO) {
            ASTNode* b = parseBoard();
            if (b) program->children.push_back(b);
            continue;
        }
        // unexpected token at top-level: record and advance
        SyntaxError e;
        e.number = ++m_errorCounter;
        e.lexeme = m_current.lexeme;
        e.errorType = "Sintactico";
        e.description = "Token inesperado en nivel superior";
        e.line = m_current.line;
        e.column = m_current.column;
        m_errors.push_back(e);
        advance();
    }
    return program;
}

ASTNode* SyntaxAnalyzer::parseBoard() {
    // TABLERO (CADENA|IDENTIFICADOR) { Column* } ;?
    Token t = advance(); // consume TABLERO
    ASTNode* node = new ASTNode{"Tablero", "", t.line, t.column, {}};
    if (m_current.type == TokenType::CADENA || m_current.type == TokenType::IDENTIFICADOR) {
        node->value = m_current.lexeme;
        advance();
    } else {
        consumeExpected(TokenType::CADENA, "Se esperaba nombre de tablero (cadena o identificador)");
    }
    consumeExpected(TokenType::LBrace, "Se esperaba '{' despues del nombre del tablero");
    while (m_current.type != TokenType::RBrace && m_current.type != TokenType::EndOfFile) {
        if (m_current.type == TokenType::COLUMNA) {
            ASTNode* col = parseColumn();
            if (col) node->children.push_back(col);
            continue;
        }
        if (m_current.type == TokenType::Semicolon || m_current.type == TokenType::Comma) {
            advance();
            continue;
        }
        // skip unexpected
        SyntaxError e;
        e.number = ++m_errorCounter;
        e.lexeme = m_current.lexeme;
        e.errorType = "Sintactico";
        e.description = "Token inesperado dentro de tablero";
        e.line = m_current.line;
        e.column = m_current.column;
        m_errors.push_back(e);
        advance();
    }
    consumeExpected(TokenType::RBrace, "Se esperaba '}' al final del tablero");
    if (m_current.type == TokenType::Semicolon) {
        advance();
    }
    return node;
}

ASTNode* SyntaxAnalyzer::parseColumn() {
    // COLUMNA (CADENA|IDENTIFICADOR) { Task* } ;?
    Token t = advance(); // consume COLUMNA
    ASTNode* node = new ASTNode{"Columna", "", t.line, t.column, {}};
    if (m_current.type == TokenType::CADENA || m_current.type == TokenType::IDENTIFICADOR) {
        node->value = m_current.lexeme; advance();
    } else {
        consumeExpected(TokenType::CADENA, "Se esperaba nombre de columna (cadena o identificador)");
    }
    consumeExpected(TokenType::LBrace, "Se esperaba '{' despues del nombre de la columna");
    while (m_current.type != TokenType::RBrace && m_current.type != TokenType::EndOfFile) {
        if (m_current.type == TokenType::TAREA) {
            ASTNode* tnode = parseTask();
            if (tnode) node->children.push_back(tnode);
            continue;
        }
        if (m_current.type == TokenType::Semicolon || m_current.type == TokenType::Comma) {
            advance();
            continue;
        }
        SyntaxError e;
        e.number = ++m_errorCounter;
        e.lexeme = m_current.lexeme;
        e.errorType = "Sintactico";
        e.description = "Token inesperado dentro de columna";
        e.line = m_current.line;
        e.column = m_current.column;
        m_errors.push_back(e);
        advance();
    }
    consumeExpected(TokenType::RBrace, "Se esperaba '}' al final de la columna");
    if (m_current.type == TokenType::Semicolon) {
        advance();
    }
    return node;
}

ASTNode* SyntaxAnalyzer::parseTask() {
    // TAREA : (CADENA|IDENTIFICADOR) [ prioridad: ALTA|MEDIA|BAJA, responsable: CADENA, fecha_limite: FECHA_LITERAL ] ,?
    Token t = advance(); // consume TAREA
    ASTNode* node = new ASTNode{"Tarea", "", t.line, t.column, {}};
    consumeExpected(TokenType::Colon, "Se esperaba ':' despues de TAREA");

    if (m_current.type == TokenType::CADENA || m_current.type == TokenType::IDENTIFICADOR) {
        node->value = m_current.lexeme;
        advance();
    } else {
        consumeExpected(TokenType::CADENA, "Se esperaba nombre de tarea (cadena o identificador)");
    }

    consumeExpected(TokenType::LBracket, "Se esperaba '[' despues del nombre de la tarea");
    while (m_current.type != TokenType::RBracket && m_current.type != TokenType::EndOfFile) {
        if (m_current.type == TokenType::PRIORIDAD) {
            advance();
            consumeExpected(TokenType::Colon, "Se esperaba ':' despues de PRIORIDAD");
            if (m_current.type == TokenType::ALTA || m_current.type == TokenType::MEDIA || m_current.type == TokenType::BAJA) {
                ASTNode* n = new ASTNode{"Prioridad", m_current.lexeme, m_current.line, m_current.column, {}};
                advance();
                node->children.push_back(n);
            } else {
                consumeExpected(TokenType::ALTA, "Se esperaba valor de prioridad (ALTA|MEDIA|BAJA)");
            }
            if (m_current.type == TokenType::Comma) advance();
            continue;
        }
        if (m_current.type == TokenType::RESPONSABLE) {
            advance();
            consumeExpected(TokenType::Colon, "Se esperaba ':' despues de RESPONSABLE");
            if (m_current.type == TokenType::CADENA) {
                ASTNode* n = new ASTNode{"Responsable", m_current.lexeme, m_current.line, m_current.column, {}};
                advance();
                node->children.push_back(n);
            } else {
                consumeExpected(TokenType::CADENA, "Se esperaba cadena para RESPONSABLE");
            }
            if (m_current.type == TokenType::Comma) advance();
            continue;
        }
        if (m_current.type == TokenType::FECHA_LIMITE) {
            advance();
            consumeExpected(TokenType::Colon, "Se esperaba ':' despues de FECHA_LIMITE");
            if (m_current.type == TokenType::FECHA_LITERAL) {
                ASTNode* n = new ASTNode{"FechaLimite", m_current.lexeme, m_current.line, m_current.column, {}};
                advance();
                node->children.push_back(n);
            } else {
                consumeExpected(TokenType::FECHA_LITERAL, "Se esperaba literal de fecha AAAA-MM-DD");
            }
            if (m_current.type == TokenType::Comma) advance();
            continue;
        }
        if (m_current.type == TokenType::Comma) {
            advance();
            continue;
        }
        // unexpected inside task
        SyntaxError e;
        e.number = ++m_errorCounter;
        e.lexeme = m_current.lexeme;
        e.errorType = "Sintactico";
        e.description = "Token inesperado dentro de tarea";
        e.line = m_current.line;
        e.column = m_current.column;
        m_errors.push_back(e);
        advance();
    }
    consumeExpected(TokenType::RBracket, "Se esperaba ']' al final de la tarea");
    if (m_current.type == TokenType::Comma) {
        advance();
    }
    return node;
}
