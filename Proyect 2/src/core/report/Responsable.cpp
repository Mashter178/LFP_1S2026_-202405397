#include "WorkloadReportGenerator.h"
#include <fstream>
#include <map>
#include <algorithm>
#include <cmath>

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

struct WorkloadData {
    int altaCount = 0;
    int mediaCount = 0;
    int bajaCount = 0;
    
    int total() const { return altaCount + mediaCount + bajaCount; }
};

static void collectWorkload(ASTNode* node, std::map<std::string, WorkloadData>& workload) {
    if (!node) return;
    
    if (node->label == "Tarea") {
        std::string responsable = "N/A";
        std::string prioridad = "MEDIA";
        
        for (auto child : node->children) {
            if (child->label == "Responsable") {
                responsable = child->value;
            } else if (child->label == "Prioridad") {
                prioridad = child->value;
            }
        }
        
        if (workload.find(responsable) == workload.end()) {
            workload[responsable] = WorkloadData();
        }
        
        if (prioridad == "ALTA") {
            workload[responsable].altaCount++;
        } else if (prioridad == "BAJA") {
            workload[responsable].bajaCount++;
        } else {
            workload[responsable].mediaCount++;
        }
    }
    
    for (auto child : node->children) {
        collectWorkload(child, workload);
    }
}

void WorkloadReportGenerator::generate(ASTNode* root, const std::string& outPath) {
    std::map<std::string, WorkloadData> workload;
    collectWorkload(root, workload);
    
    // Calcular total de tareas
    int totalTasks = 0;
    for (auto& pair : workload) {
        totalTasks += pair.second.total();
    }
    
    std::ofstream f(outPath);
    f << R"(<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Carga de Trabajo por Responsable</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #1e3c72 0%, #2a5298 100%);
            padding: 40px 20px;
            min-height: 100vh;
        }
        .container {
            max-width: 1200px;
            margin: 0 auto;
        }
        h1 {
            color: white;
            text-align: center;
            margin-bottom: 10px;
            font-size: 2.5em;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
        }
        .subtitle {
            color: rgba(255,255,255,0.8);
            text-align: center;
            margin-bottom: 40px;
            font-size: 1.1em;
        }
        .summary-bar {
            background: white;
            border-radius: 8px;
            padding: 20px;
            margin-bottom: 30px;
            box-shadow: 0 4px 15px rgba(0,0,0,0.2);
            display: flex;
            justify-content: space-around;
            flex-wrap: wrap;
            gap: 20px;
        }
        .summary-item {
            text-align: center;
        }
        .summary-number {
            font-size: 2.5em;
            font-weight: bold;
            color: #2a5298;
        }
        .summary-label {
            color: #666;
            font-size: 0.95em;
            margin-top: 5px;
        }
        .workload-grid {
            display: grid;
            gap: 20px;
            grid-template-columns: repeat(auto-fit, minmax(350px, 1fr));
        }
        .workload-card {
            background: white;
            border-radius: 8px;
            padding: 20px;
            box-shadow: 0 4px 15px rgba(0,0,0,0.15);
            transition: transform 0.3s ease, box-shadow 0.3s ease;
        }
        .workload-card:hover {
            transform: translateY(-5px);
            box-shadow: 0 6px 20px rgba(0,0,0,0.25);
        }
        .responsable-name {
            font-size: 1.4em;
            font-weight: bold;
            color: #2a5298;
            margin-bottom: 15px;
            display: flex;
            align-items: center;
            gap: 10px;
        }
        .task-count {
            background: #2a5298;
            color: white;
            padding: 3px 8px;
            border-radius: 20px;
            font-size: 0.9em;
            font-weight: bold;
        }
        .priority-breakdown {
            display: flex;
            justify-content: space-between;
            margin-bottom: 20px;
            gap: 10px;
        }
        .priority-item {
            flex: 1;
            text-align: center;
            padding: 12px;
            border-radius: 6px;
            font-weight: bold;
        }
        .priority-item.alta {
            background: #ffe0e0;
            color: #cc0000;
            border-left: 4px solid #ff6b6b;
        }
        .priority-item.media {
            background: #fff4e0;
            color: #cc6600;
            border-left: 4px solid #ffa500;
        }
        .priority-item.baja {
            background: #e0f5e3;
            color: #006600;
            border-left: 4px solid #51cf66;
        }
        .priority-count {
            font-size: 1.8em;
            display: block;
            margin-bottom: 5px;
        }
        .priority-label {
            font-size: 0.85em;
        }
        .progress-bar {
            background: #e0e0e0;
            border-radius: 8px;
            height: 12px;
            overflow: hidden;
            margin-bottom: 10px;
        }
        .progress-fill {
            height: 100%;
            background: linear-gradient(90deg, #ff6b6b 0%, #ffa500 50%, #51cf66 100%);
            border-radius: 8px;
            transition: width 0.5s ease;
        }
        .progress-info {
            font-size: 0.85em;
            color: #666;
            display: flex;
            justify-content: space-between;
        }
        .empty-state {
            grid-column: 1 / -1;
            text-align: center;
            padding: 60px 20px;
            color: rgba(255,255,255,0.6);
        }
        .empty-state svg {
            margin-bottom: 20px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>👥 Carga de Trabajo por Responsable</h1>
        <div class="subtitle">Distribución de tareas y prioridades</div>
        
        <div class="summary-bar">
            <div class="summary-item">
                <div class="summary-number">)";
    
    f << totalTasks;
    
    f << R"(</div>
                <div class="summary-label">Total de Tareas</div>
            </div>
            <div class="summary-item">
                <div class="summary-number">)";
    
    f << workload.size();
    
    f << R"(</div>
                <div class="summary-label">Responsables</div>
            </div>
            <div class="summary-item">
                <div class="summary-number">)";
    
    int altaTotal = 0, mediaTotal = 0, bajaTotal = 0;
    for (auto& pair : workload) {
        altaTotal += pair.second.altaCount;
        mediaTotal += pair.second.mediaCount;
        bajaTotal += pair.second.bajaCount;
    }
    f << altaTotal;
    
    f << R"(</div>
                <div class="summary-label">Tareas ALTA Prioridad</div>
            </div>
        </div>
)";
    
    if (workload.empty()) {
        f << R"(        <div class="empty-state">
            <h2>Sin datos de carga</h2>
            <p>No hay tareas asignadas</p>
        </div>
)";
    } else {
        f << "        <div class=\"workload-grid\">\n";
        
        for (auto& pair : workload) {
            std::string name = pair.first;
            WorkloadData& data = pair.second;
            int total = data.total();
            double percentage = (totalTasks > 0) ? (100.0 * total / totalTasks) : 0;
            
            f << "            <div class=\"workload-card\">\n";
            f << "                <div class=\"responsable-name\">\n";
            f << "                    👤 " << escapeHtml(name) << "\n";
            f << "                    <span class=\"task-count\">" << total << "</span>\n";
            f << "                </div>\n";
            
            f << "                <div class=\"priority-breakdown\">\n";
            f << "                    <div class=\"priority-item alta\">\n";
            f << "                        <span class=\"priority-count\">" << data.altaCount << "</span>\n";
            f << "                        <span class=\"priority-label\">ALTA</span>\n";
            f << "                    </div>\n";
            f << "                    <div class=\"priority-item media\">\n";
            f << "                        <span class=\"priority-count\">" << data.mediaCount << "</span>\n";
            f << "                        <span class=\"priority-label\">MEDIA</span>\n";
            f << "                    </div>\n";
            f << "                    <div class=\"priority-item baja\">\n";
            f << "                        <span class=\"priority-count\">" << data.bajaCount << "</span>\n";
            f << "                        <span class=\"priority-label\">BAJA</span>\n";
            f << "                    </div>\n";
            f << "                </div>\n";
            
            f << "                <div class=\"progress-bar\">\n";
            f << "                    <div class=\"progress-fill\" style=\"width: " << percentage << "%\"></div>\n";
            f << "                </div>\n";
            f << "                <div class=\"progress-info\">\n";
            f << "                    <span>" << std::fixed << percentage;
            f.precision(1);
            f << "% del total</span>\n";
            f << "                </div>\n";
            f << "            </div>\n";
        }
        
        f << "        </div>\n";
    }
    
    f << R"(    </div>
</body>
</html>)";
}
