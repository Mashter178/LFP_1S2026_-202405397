#include "KanbanReportGenerator.h"
#include <fstream>
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

struct TaskCard {
    std::string name;
    std::string prioridad;
    std::string responsable;
    std::string fechaLimite;
    std::string columna;
};

static std::string extractTaskAttribute(ASTNode* taskNode, const std::string& attrName) {
    if (!taskNode) return "";
    for (auto child : taskNode->children) {
        if (child->label == attrName) {
            return child->value;
        }
    }
    return "";
}

static void collectTasks(ASTNode* node, std::vector<TaskCard>& tasks) {
    if (!node) return;
    
    if (node->label == "Columna") {
        std::string columnName = node->value;
        for (auto child : node->children) {
            if (child->label == "Tarea") {
                TaskCard card;
                card.name = child->value;
                card.columna = columnName;
                card.prioridad = extractTaskAttribute(child, "Prioridad");
                card.responsable = extractTaskAttribute(child, "Responsable");
                card.fechaLimite = extractTaskAttribute(child, "FechaLimite");
                if (card.prioridad.empty()) card.prioridad = "MEDIA";
                if (card.responsable.empty()) card.responsable = "N/A";
                if (card.fechaLimite.empty()) card.fechaLimite = "N/A";
                tasks.push_back(card);
            }
        }
    }
    
    for (auto child : node->children) {
        collectTasks(child, tasks);
    }
}

void KanbanReportGenerator::generate(ASTNode* root, const std::string& outPath) {
    std::vector<TaskCard> tasks;
    collectTasks(root, tasks);
    
    std::ofstream f(outPath);
    f << R"(<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Tablero Kanban</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            padding: 30px;
            min-height: 100vh;
        }
        .container {
            max-width: 1400px;
            margin: 0 auto;
        }
        h1 {
            color: white;
            text-align: center;
            margin-bottom: 40px;
            font-size: 2.5em;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
        }
        .kanban-board {
            display: flex;
            gap: 20px;
            justify-content: space-between;
        }
        .column {
            flex: 1;
            background: #f8f9fa;
            border-radius: 8px;
            padding: 20px;
            box-shadow: 0 4px 15px rgba(0,0,0,0.1);
            max-height: 80vh;
            overflow-y: auto;
        }
        .column-header {
            font-weight: bold;
            font-size: 1.3em;
            margin-bottom: 20px;
            padding-bottom: 12px;
            border-bottom: 3px solid #667eea;
            color: #333;
        }
        .column.todo .column-header { border-bottom-color: #ff6b6b; }
        .column.progress .column-header { border-bottom-color: #ffa500; }
        .column.done .column-header { border-bottom-color: #51cf66; }
        
        .task-card {
            background: white;
            border-radius: 6px;
            padding: 15px;
            margin-bottom: 15px;
            box-shadow: 0 2px 8px rgba(0,0,0,0.08);
            border-left: 5px solid #ccc;
            transition: all 0.3s ease;
            cursor: pointer;
        }
        .task-card:hover {
            transform: translateY(-3px);
            box-shadow: 0 4px 12px rgba(0,0,0,0.15);
        }
        .task-card.alta { border-left-color: #ff6b6b; background: #ffe0e0; }
        .task-card.media { border-left-color: #ffa500; background: #fff4e0; }
        .task-card.baja { border-left-color: #51cf66; background: #e0f5e3; }
        
        .task-name {
            font-weight: bold;
            font-size: 1.1em;
            margin-bottom: 10px;
            color: #333;
            word-break: break-word;
        }
        .task-priority {
            display: inline-block;
            padding: 4px 10px;
            border-radius: 4px;
            font-weight: bold;
            font-size: 0.85em;
            margin-bottom: 8px;
            color: white;
        }
        .task-priority.alta { background: #ff6b6b; }
        .task-priority.media { background: #ffa500; }
        .task-priority.baja { background: #51cf66; }
        
        .task-info {
            font-size: 0.9em;
            color: #666;
            margin-bottom: 5px;
        }
        .task-info strong { color: #333; }
        .empty-column {
            text-align: center;
            color: #999;
            padding: 40px 20px;
            font-style: italic;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>📋 Tablero Kanban</h1>
        <div class="kanban-board">
)";

    std::map<std::string, std::vector<TaskCard>> byColumn;
    for (auto& task : tasks) {
        byColumn[task.columna].push_back(task);
    }
    
    std::vector<std::string> columnOrder = {"Por Hacer", "En Progreso", "Completado"};
    std::vector<std::string> columnClasses = {"todo", "progress", "done"};
    
    for (size_t i = 0; i < columnOrder.size(); i++) {
        std::string colName = columnOrder[i];
        std::string colClass = columnClasses[i];
        
        f << "            <div class=\"column " << colClass << "\">\n";
        f << "                <div class=\"column-header\">" << colName << " (" << byColumn[colName].size() << ")</div>\n";
        
        if (byColumn[colName].empty()) {
            f << "                <div class=\"empty-column\">Sin tareas</div>\n";
        } else {
            for (auto& task : byColumn[colName]) {
                std::string priorityLower = task.prioridad;
                std::transform(priorityLower.begin(), priorityLower.end(), priorityLower.begin(), ::tolower);
                
                f << "                <div class=\"task-card " << priorityLower << "\">\n";
                f << "                    <div class=\"task-name\">" << escapeHtml(task.name) << "</div>\n";
                f << "                    <span class=\"task-priority " << priorityLower << "\">" << task.prioridad << "</span>\n";
                f << "                    <div class=\"task-info\">👤 <strong>" << escapeHtml(task.responsable) << "</strong></div>\n";
                f << "                    <div class=\"task-info\">📅 <strong>" << task.fechaLimite << "</strong></div>\n";
                f << "                </div>\n";
            }
        }
        
        f << "            </div>\n";
    }
    
    f << R"(        </div>
    </div>
</body>
</html>)";
}
