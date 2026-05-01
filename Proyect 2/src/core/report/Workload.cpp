#include "Workload.h"
#include <fstream>
#include <map>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <sstream>

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

void Workload::generate(ASTNode* root, const std::string& outPath) {
    std::map<std::string, WorkloadData> workload;
    
    if (root) {
        collectWorkload(root, workload);
    }
    
    int totalTasks = 0;
    int altaTotal = 0, mediaTotal = 0, bajaTotal = 0;
    for (auto& pair : workload) {
        totalTasks += pair.second.total();
        altaTotal += pair.second.altaCount;
        mediaTotal += pair.second.mediaCount;
        bajaTotal += pair.second.bajaCount;
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
            max-width: 1000px;
            margin: 0 auto;
        }
        h1 {
            color: white;
            text-align: center;
            margin-bottom: 30px;
            font-size: 2.2em;
            text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
        }
        .summary {
            background: white;
            border-radius: 8px;
            padding: 20px;
            margin-bottom: 30px;
            box-shadow: 0 4px 15px rgba(0,0,0,0.2);
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
            gap: 20px;
        }
        .summary-item {
            text-align: center;
        }
        .summary-number {
            font-size: 2em;
            font-weight: bold;
            color: #2a5298;
        }
        .summary-label {
            color: #666;
            font-size: 0.9em;
            margin-top: 5px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            background: white;
            border-radius: 8px;
            overflow: hidden;
            box-shadow: 0 4px 15px rgba(0,0,0,0.2);
        }
        th {
            background: #2a5298;
            color: white;
            padding: 15px;
            text-align: left;
            font-weight: bold;
        }
        td {
            padding: 12px 15px;
            border-bottom: 1px solid #e0e0e0;
        }
        tr:last-child td {
            border-bottom: none;
        }
        tr:hover {
            background: #f5f5f5;
        }
        .responsable-cell {
            font-weight: 500;
            color: #2a5298;
        }
        .count-alta {
            background: #ffe0e0;
            color: #cc0000;
            padding: 5px 10px;
            border-radius: 4px;
            font-weight: bold;
            text-align: center;
        }
        .count-media {
            background: #fff4e0;
            color: #cc6600;
            padding: 5px 10px;
            border-radius: 4px;
            font-weight: bold;
            text-align: center;
        }
        .count-baja {
            background: #e0f5e3;
            color: #006600;
            padding: 5px 10px;
            border-radius: 4px;
            font-weight: bold;
            text-align: center;
        }
        .count-total {
            background: #e3f2fd;
            color: #1565c0;
            padding: 5px 10px;
            border-radius: 4px;
            font-weight: bold;
            text-align: center;
        }
        .percentage {
            color: #666;
            font-size: 0.9em;
        }
        .empty-message {
            text-align: center;
            color: white;
            padding: 40px;
            font-size: 1.1em;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Carga de Trabajo por Responsable</h1>
        
        <div class="summary">
            <div class="summary-item">
                <div class="summary-number">)";
    
    f << totalTasks;
    f << R"(</div>
                <div class="summary-label">Total Tareas</div>
            </div>
            <div class="summary-item">
                <div class="summary-number">)";
    
    f << workload.size();
    f << R"(</div>
                <div class="summary-label">Responsables</div>
            </div>
            <div class="summary-item">
                <div class="summary-number">)";
    
    f << altaTotal;
    f << R"(</div>
                <div class="summary-label">ALTA</div>
            </div>
            <div class="summary-item">
                <div class="summary-number">)";
    
    f << mediaTotal;
    f << R"(</div>
                <div class="summary-label">MEDIA</div>
            </div>
            <div class="summary-item">
                <div class="summary-number">)";
    
    f << bajaTotal;
    f << R"(</div>
                <div class="summary-label">BAJA</div>
            </div>
        </div>
)";
    
    if (workload.empty()) {
        f << R"(        <div class="empty-message">Sin datos de carga de trabajo</div>
)";
    } else {
        f << R"(        <table>
            <thead>
                <tr>
                    <th>Responsable</th>
                    <th style="text-align: center; width: 80px;">ALTA</th>
                    <th style="text-align: center; width: 80px;">MEDIA</th>
                    <th style="text-align: center; width: 80px;">BAJA</th>
                    <th style="text-align: center; width: 100px;">Total</th>
                    <th style="text-align: center; width: 120px;">% Carga</th>
                </tr>
            </thead>
            <tbody>
)";
        
        for (auto& pair : workload) {
            std::string name = pair.first;
            WorkloadData& data = pair.second;
            int total = data.total();
            double percentage = (totalTasks > 0) ? (100.0 * total / totalTasks) : 0;
            
            f << "                <tr>\n";
            f << "                    <td class=\"responsable-cell\">" << escapeHtml(name) << "</td>\n";
            f << "                    <td><span class=\"count-alta\">" << data.altaCount << "</span></td>\n";
            f << "                    <td><span class=\"count-media\">" << data.mediaCount << "</span></td>\n";
            f << "                    <td><span class=\"count-baja\">" << data.bajaCount << "</span></td>\n";
            f << "                    <td><span class=\"count-total\">" << total << "</span></td>\n";
            
            // Format percentage with 1 decimal place using stringstream
            std::ostringstream percStream;
            percStream << std::fixed << std::setprecision(1) << percentage;
            f << "                    <td><span class=\"percentage\">" << percStream.str() << "%</span></td>\n";
            
            f << "                </tr>\n";
        }
        
        f << R"(            </tbody>
        </table>
)";
    }
    
    f << R"(    </div>
</body>
</html>)";
}
