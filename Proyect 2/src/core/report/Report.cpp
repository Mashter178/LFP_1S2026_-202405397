#include "ReportGenerator.h"
#include "KanbanReportGenerator.h"
#include "WorkloadReportGenerator.h"
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

void ReportGenerator::generateTokenReport(const std::vector<Token>& tokens, const std::string& outPath) {
    std::ofstream f(outPath);
    f << "<html><head><meta charset=\"utf-8\"><title>Tokens</title><style>table{border-collapse:collapse}td,th{border:1px solid #ccc;padding:6px}</style></head><body>";
    f << "<h1>Tokens</h1>\n<table><tr><th>#</th><th>Lexema</th><th>Tipo</th><th>Linea</th><th>Columna</th></tr>";
    int i=1; for (auto &t: tokens) { f << "<tr><td>"<<i++<<"</td><td>"<<escapeHtml(t.lexeme)<<"</td><td>"<<escapeHtml(tokenTypeToString(t.type))<<"</td><td>"<<t.line<<"</td><td>"<<t.column<<"</td></tr>\n"; }
    f << "</table></body></html>";
}

void ReportGenerator::generateErrorReport(const std::vector<LexicalError>& lexErrors, const std::vector<SyntaxError>& synErrors, const std::vector<SemanticError>& semErrors, const std::string& outPath) {
    std::ofstream f(outPath);
    f << "<html><head><meta charset=\"utf-8\"><title>Errores</title><style>table{border-collapse:collapse}td,th{border:1px solid #ccc;padding:6px}</style></head><body>";
    f << "<h1>Errores L&eacute;xicos</h1>\n<table><tr><th>#</th><th>Lexema</th><th>Tipo</th><th>Descripci&oacute;n</th><th>L&iacute;nea</th><th>Columna</th></tr>";
    int i=1; for (auto &e: lexErrors) { f << "<tr><td>"<<i++<<"</td><td>"<<escapeHtml(e.invalidLexeme)<<"</td><td>"<<escapeHtml(e.errorType)<<"</td><td>"<<escapeHtml(e.description)<<"</td><td>"<<e.line<<"</td><td>"<<e.column<<"</td></tr>\n"; }
    f << "</table>\n<h1>Errores Sint&aacute;cticos</h1>\n<table><tr><th>#</th><th>Lexema</th><th>Tipo</th><th>Descripci&oacute;n</th><th>L&iacute;nea</th><th>Columna</th></tr>";
    i=1; for (auto &e: synErrors) { f << "<tr><td>"<<i++<<"</td><td>"<<escapeHtml(e.lexeme)<<"</td><td>"<<escapeHtml(e.errorType)<<"</td><td>"<<escapeHtml(e.description)<<"</td><td>"<<e.line<<"</td><td>"<<e.column<<"</td></tr>\n"; }
    f << "</table>\n<h1>Errores Sem&aacute;nticos</h1>\n<table><tr><th>#</th><th>Lexema</th><th>Tipo</th><th>Descripci&oacute;n</th><th>L&iacute;nea</th><th>Columna</th></tr>";
    i=1; for (auto &e: semErrors) { f << "<tr><td>"<<i++<<"</td><td>"<<escapeHtml(e.lexeme)<<"</td><td>"<<escapeHtml(e.errorType)<<"</td><td>"<<escapeHtml(e.description)<<"</td><td>"<<e.line<<"</td><td>"<<e.column<<"</td></tr>\n"; }
    f << "</table></body></html>";
}

void ReportGenerator::generateStatsReport(const std::vector<Token>& tokens, const std::vector<LexicalError>& lexErrors, const std::vector<SyntaxError>& synErrors, const std::vector<SemanticError>& semErrors, const std::string& outPath) {
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

void ReportGenerator::generateAstDot(ASTNode* root, const std::string& outPath) {
    std::ofstream f(outPath);
    f << "digraph AST {\nnode [shape=box];\n";
    if (root) dumpAstNode(f, root);
    f << "}\n";
}

void ReportGenerator::generateKanbanReport(ASTNode* root, const std::string& outPath) {
    KanbanReportGenerator::generate(root, outPath);
}

void ReportGenerator::generateWorkloadReport(ASTNode* root, const std::string& outPath) {
    WorkloadReportGenerator::generate(root, outPath);
}
