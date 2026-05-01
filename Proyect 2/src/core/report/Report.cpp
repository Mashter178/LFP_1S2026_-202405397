#include "Report.h"
#include "Kanban.h"
#include "Workload.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <map>
#include <algorithm>

static std::string escapeHtml(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '&': out += "&amp;"; break;
            case '<': out += "&lt;"; break;
            case '>': out += "&gt;"; break;
            case '"': out += "&quot;"; break;
            default: out.push_back(c); break;
        }
    }
    return out;
}

void Report::generateTokenReport(const std::vector<Token>& tokens, const std::string& outPath) {
    std::ofstream f(outPath);
    f << "<html><head><meta charset=\"utf-8\"><title>Tokens</title><style>table{border-collapse:collapse}td,th{border:1px solid #ccc;padding:6px}</style></head><body>";
    f << "<h1>Tokens</h1>\n<table><tr><th>#</th><th>Lexema</th><th>Tipo</th><th>Linea</th><th>Columna</th></tr>";
    int i=1; for (auto &t: tokens) { f << "<tr><td>"<<i++<<"</td><td>"<<escapeHtml(t.lexeme)<<"</td><td>"<<escapeHtml(tokenTypeToString(t.type))<<"</td><td>"<<t.line<<"</td><td>"<<t.column<<"</td></tr>\n"; }
    f << "</table></body></html>";
}

void Report::generateErrorReport(const std::vector<LexicalError>& lexErrors, const std::vector<SyntaxError>& synErrors, const std::vector<SemanticError>& semErrors, const std::string& outPath) {
    struct ErrorRow { std::string lexeme; std::string tokenOrType; std::string errorKind; std::string description; int line; int column; };
    std::vector<ErrorRow> rows;

    for (auto &e: lexErrors) {
        rows.push_back({ e.invalidLexeme, std::string(), std::string("Léxico"), e.description, e.line, e.column });
    }
    for (auto &e: synErrors) {
        rows.push_back({ e.lexeme, std::string(), std::string("Sintáctico"), e.description, e.line, e.column });
    }
    for (auto &e: semErrors) {
        rows.push_back({ e.lexeme, std::string(), std::string("Semántico"), e.description, e.line, e.column });
    }

    // ordenar por línea y columna
    std::sort(rows.begin(), rows.end(), [](const ErrorRow &a, const ErrorRow &b){
        if (a.line != b.line) return a.line < b.line;
        return a.column < b.column;
    });

    std::ofstream f(outPath);
    f << "<html><head><meta charset=\"utf-8\"><title>Errores</title><style>table{border-collapse:collapse;width:100%}th,td{border:1px solid #ccc;padding:8px;text-align:left}th{background:#2a5298;color:#fff}</style></head><body>";
    f << "<h1>Tabla Unificada de Errores (Léxicos / Sintácticos / Semánticos)</h1>\n";
    f << "<table><tr><th>N&deg;</th><th>Lexema / Token</th><th>Tipo de error</th><th>Descripci&oacute;n</th><th>L&iacute;nea:Columna</th></tr>";
    int idx = 1;
    for (auto &r: rows) {
        std::ostringstream pos;
        pos << r.line << ":" << r.column;
        f << "<tr><td>"<< idx++ <<"</td><td>"<< escapeHtml(r.lexeme) <<"</td><td>"<< escapeHtml(r.errorKind) <<"</td><td>"<< escapeHtml(r.description) <<"</td><td>"<< pos.str() <<"</td></tr>\n";
    }
    f << "</table></body></html>";
}

void Report::generateStatsReport(const std::vector<Token>& tokens, const std::vector<LexicalError>& lexErrors, const std::vector<SyntaxError>& synErrors, const std::vector<SemanticError>& semErrors, const std::string& outPath) {
    std::ofstream f(outPath);
    std::unordered_map<std::string,int> counts;
    for (auto &t: tokens) counts[tokenTypeToString(t.type)]++;
    f << "<html><head><meta charset=\"utf-8\"><title>Estadisticas</title><style>table{border-collapse:collapse}td,th{border:1px solid #ccc;padding:6px}</style></head><body>";
    f << "<h1>Estadisticas</h1>\n<table><tr><th>Elemento</th><th>Conteo</th></tr>";
    for (auto &p: counts) f << "<tr><td>"<<escapeHtml(p.first)<<"</td><td>"<<p.second<<"</td></tr>\n";
    f << "</table>\n<p>Errores L&eacute;xicos: "<<lexErrors.size()<<"</p>\n<p>Errores Sint&aacute;cticos: "<<synErrors.size()<<"</p>\n</body></html>";
}

static int dumpAstNode(std::ofstream &f, ASTNode* node) {
    static int id = 0;
    int myId = ++id;
    f << "  node"<<myId<<" [label=\""<<node->label<<"\n"<<node->value<<"\"];";
    f << "\n";
    for (auto c: node->children) {
        int cid = dumpAstNode(f,c);
        f << "  node"<<myId<<" -> node"<<cid<<";\n";
    }
    return myId;
}

void Report::generateAstDot(ASTNode* root, const std::string& outPath) {
    std::ofstream f(outPath);
    f << "digraph AST {\nnode [shape=box];\n";
    if (root) dumpAstNode(f, root);
    f << "}\n";
}

void Report::generateKanbanReport(ASTNode* root, const std::string& outPath) {
    Kanban::generate(root, outPath);
}

void Report::generateWorkloadReport(ASTNode* root, const std::string& outPath) {
    Workload::generate(root, outPath);
}
