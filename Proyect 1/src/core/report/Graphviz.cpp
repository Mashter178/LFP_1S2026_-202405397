#include "Graphviz.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

namespace {

std::string dotEscape(const std::string& text) {
    std::string out;
    out.reserve(text.size());
    for (char c : text) {
        if (c == '\\') {
            out += "\\\\";
        } else if (c == '"') {
            out += "\\\"";
        } else {
            out += c;
        }
    }
    return out;
}

std::string nodeId(const char prefix, std::size_t index) {
    return std::string(1, prefix) + std::to_string(index + 1);
}

}

bool writeHospitalGraphvizDot(
    const std::string& outputPath,
    const Hospital& data,
    const std::string& sourceFilePath
) {
    const std::filesystem::path path(outputPath);
    if (path.has_parent_path()) {
        std::error_code errorCode;
        std::filesystem::create_directories(path.parent_path(), errorCode);
    }

    std::ofstream out(outputPath.c_str(), std::ios::out | std::ios::trunc);
    if (!out) {
        return false;
    }

    const std::string hospitalName = std::filesystem::path(sourceFilePath).stem().string();
    const std::string rootLabel = hospitalName.empty() ? "Hospital Analizado" : hospitalName;

    std::unordered_map<std::string, std::string> patientNodeByName;
    std::unordered_map<std::string, std::string> doctorNodeByName;
    std::unordered_map<std::string, int> scheduleCounts;

    for (const Cita& cita : data.citas) {
        const std::string key = cita.medico + "|" + cita.fecha + "|" + cita.hora;
        scheduleCounts[key]++;
    }

    out << "digraph Hospital {\n";
    out << "rankdir=TB;\n";
    out << "node [shape=box, style=filled, fontname=\"Arial\"];\n";
    out << "H [label=\"" << dotEscape(rootLabel) << "\", fillcolor=\"#1A4731\", fontcolor=white, shape=ellipse];\n";
    out << "P [label=\"PACIENTES\", fillcolor=\"#2E7D52\", fontcolor=white];\n";
    out << "M [label=\"MEDICOS\", fillcolor=\"#2E7D52\", fontcolor=white];\n";
    out << "C [label=\"CITAS\", fillcolor=\"#2E7D52\", fontcolor=white];\n";
    out << "D [label=\"DIAGNOSTICOS\", fillcolor=\"#2E7D52\", fontcolor=white];\n";
    out << "H -> P; H -> M; H -> C; H -> D;\n";

    for (std::size_t i = 0; i < data.pacientes.size(); ++i) {
        const Paciente& paciente = data.pacientes[i];
        const std::string id = nodeId('p', i);
        patientNodeByName[paciente.nombre] = id;

        std::ostringstream label;
        label << paciente.nombre << "\\n" << paciente.tipoSangre << " | Hab. " << paciente.habitacion;
        out << id << " [label=\"" << dotEscape(label.str()) << "\", fillcolor=\"#D4EDDA\"];\n";
        out << "P -> " << id << ";\n";
    }

    for (std::size_t i = 0; i < data.medicos.size(); ++i) {
        const Medico& medico = data.medicos[i];
        const std::string id = nodeId('m', i);
        doctorNodeByName[medico.nombre] = id;

        std::ostringstream label;
        label << medico.nombre << "\\n" << medico.codigo << " | " << medico.especialidad;
        out << id << " [label=\"" << dotEscape(label.str()) << "\", fillcolor=\"#D6EAF8\"];\n";
        out << "M -> " << id << ";\n";
    }

    for (std::size_t i = 0; i < data.citas.size(); ++i) {
        const Cita& cita = data.citas[i];
        const auto pIt = patientNodeByName.find(cita.paciente);
        const auto mIt = doctorNodeByName.find(cita.medico);
        if (pIt == patientNodeByName.end() || mIt == doctorNodeByName.end()) {
            continue;
        }

        const std::string key = cita.medico + "|" + cita.fecha + "|" + cita.hora;
        const bool conflict = scheduleCounts[key] > 1;
        const std::string edgeColor = conflict ? "#C0392B" : "#E67E22";

        out << pIt->second << " -> " << mIt->second
            << " [label=\"" << dotEscape(cita.fecha + "  " + cita.hora) << "\", color=\"" << edgeColor << "\", style=dashed];\n";
    }

    for (std::size_t i = 0; i < data.diagnosticos.size(); ++i) {
        const Diagnostico& diagnostico = data.diagnosticos[i];
        const std::string id = nodeId('d', i);

        std::ostringstream label;
        label << diagnostico.condicion << "\\n" << diagnostico.medicamento << " / " << diagnostico.dosis;
        out << id << " [label=\"" << dotEscape(label.str()) << "\", fillcolor=\"#FDEBD0\"];\n";
        out << "D -> " << id << ";\n";

        const auto pIt = patientNodeByName.find(diagnostico.paciente);
        if (pIt != patientNodeByName.end()) {
            out << id << " -> " << pIt->second << " [label=\"diagnostico activo\", color=\"#C0392B\"];\n";
        }
    }

    out << "}\n";
    return true;
}
