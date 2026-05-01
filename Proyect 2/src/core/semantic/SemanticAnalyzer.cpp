#include "SemanticAnalyzer.h"
#include <unordered_set>

SemanticAnalyzer::SemanticAnalyzer() {}

std::vector<SemanticError> SemanticAnalyzer::analyze(ASTNode* root) {
    m_errors.clear(); m_counter = 0;
    if (!root) return m_errors;
    analyzeProgram(root);
    return m_errors;
}

void SemanticAnalyzer::analyzeProgram(ASTNode* program) {
    if (!program) return;
    std::unordered_set<std::string> boardNames;
    for (auto child : program->children) {
        if (!child || child->label != "Tablero") {
            continue;
        }

        if (boardNames.find(child->value) != boardNames.end()) {
            addError(child->value, "Nombre de tablero duplicado", child->line, child->column);
        } else {
            boardNames.insert(child->value);
        }

        analyzeBoard(child);
    }
}

void SemanticAnalyzer::analyzeBoard(ASTNode* board) {
    if (!board) return;
    std::unordered_set<std::string> columnNames;
    for (auto c : board->children) {
        if (!c) continue;
        if (c->label == "Columna") {
            if (columnNames.find(c->value) != columnNames.end()) {
                addError(c->value, "Nombre de columna duplicado en el mismo tablero", c->line, c->column);
            } else {
                columnNames.insert(c->value);
            }
            analyzeColumn(c);
        }
    }
}

void SemanticAnalyzer::analyzeColumn(ASTNode* column) {
    if (!column) return;
    std::unordered_set<std::string> taskNames;
    for (auto t : column->children) {
        if (!t) continue;
        if (t->label == "Tarea") {
            if (taskNames.find(t->value) != taskNames.end()) {
                addError(t->value, "Nombre de tarea duplicado en la misma columna", t->line, t->column);
            } else {
                taskNames.insert(t->value);
            }
            analyzeTask(t);
        }
    }
}

void SemanticAnalyzer::analyzeTask(ASTNode* task) {
    if (!task) return;

    int prioridadCount = 0;
    int responsableCount = 0;
    int fechaLimiteCount = 0;

    for (auto p : task->children) {
        if (!p) continue;
        if (p->label == "Prioridad") prioridadCount++;
        else if (p->label == "Responsable") responsableCount++;
        else if (p->label == "FechaLimite") fechaLimiteCount++;
    }

    if (prioridadCount == 0) {
        addError(task->value, "La tarea no tiene atributo PRIORIDAD", task->line, task->column);
    } else if (prioridadCount > 1) {
        addError(task->value, "La tarea tiene PRIORIDAD duplicada", task->line, task->column);
    }

    if (responsableCount == 0) {
        addError(task->value, "La tarea no tiene atributo RESPONSABLE", task->line, task->column);
    } else if (responsableCount > 1) {
        addError(task->value, "La tarea tiene RESPONSABLE duplicado", task->line, task->column);
    }

    if (fechaLimiteCount == 0) {
        addError(task->value, "La tarea no tiene atributo FECHA_LIMITE", task->line, task->column);
    } else if (fechaLimiteCount > 1) {
        addError(task->value, "La tarea tiene FECHA_LIMITE duplicada", task->line, task->column);
    }
}

void SemanticAnalyzer::addError(const std::string& lex, const std::string& desc, int line, int col) {
    SemanticError e;
    e.number = ++m_counter;
    e.lexeme = lex;
    e.errorType = "Semantico";
    e.description = desc;
    e.line = line;
    e.column = col;
    m_errors.push_back(e);
}
