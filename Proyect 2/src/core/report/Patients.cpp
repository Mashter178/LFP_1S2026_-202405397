#include "Patients.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "Html.h"

namespace {

std::string toLowerAscii(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

bool isCriticalDiagnosis(const Diagnostico& diagnostico) {
    const std::string condition = toLowerAscii(diagnostico.condicion);
    const std::string dose = toLowerAscii(diagnostico.dosis);

    const char* criticalTerms[] = {"crit", "grave", "sever", "urgenc", "shock", "uci", "inestab", "aguda"};
    for (const char* term : criticalTerms) {
        if (condition.find(term) != std::string::npos) {
            return true;
        }
    }

    return dose.find("cada_1") != std::string::npos
        || dose.find("cada_2") != std::string::npos
        || dose.find("cada_3") != std::string::npos
        || dose.find("cada_4") != std::string::npos;
}

}

bool writePatientHistoryHtmlReport(const std::string& outputPath, const Hospital& data) {
    std::unordered_map<std::string, std::vector<Diagnostico> > diagnosticosPorPaciente;

    for (std::size_t i = 0; i < data.diagnosticos.size(); ++i) {
        diagnosticosPorPaciente[data.diagnosticos[i].paciente].push_back(data.diagnosticos[i]);
    }

    std::ostringstream body;
    body << "<div class=\"section card\">";
    body << "<h2 class=\"section-title\">Historial de pacientes</h2>";

    if (data.pacientes.empty()) {
        body << "<p class=\"muted\">No hay pacientes cargados para construir el historial.</p>";
    } else {
        body << "<table><thead><tr><th>Paciente</th><th>Edad</th><th>Sangre</th><th>Diagnostico activo</th><th>Medicamento / Dosis</th><th>Estado</th></tr></thead><tbody>";
        for (std::size_t i = 0; i < data.pacientes.size(); ++i) {
            const Paciente& paciente = data.pacientes[i];
            const std::vector<Diagnostico>& diagnosticos = diagnosticosPorPaciente[paciente.nombre];
            const Diagnostico* activeDiagnosis = diagnosticos.empty() ? nullptr : &diagnosticos.back();

            std::string statusText = "SIN DIAG.";
            std::string statusClass = "status-warn";
            std::string conditionText = "Sin diagnostico registrado";
            std::string medicationText = "-";

            if (activeDiagnosis != nullptr) {
                conditionText = activeDiagnosis->condicion;
                medicationText = activeDiagnosis->medicamento + " / " + activeDiagnosis->dosis;
                if (isCriticalDiagnosis(*activeDiagnosis)) {
                    statusText = "CRITICO";
                    statusClass = "status-danger";
                } else {
                    statusText = "ACTIVO";
                    statusClass = "status-ok";
                }
            }

            body << "<tr>";
            body << "<td>" << htmlEscape(paciente.nombre) << "</td>";
            body << "<td>" << paciente.edad << "</td>";
            body << "<td>" << htmlEscape(paciente.tipoSangre) << "</td>";
            body << "<td>" << htmlEscape(conditionText) << "</td>";
            body << "<td>" << htmlEscape(medicationText) << "</td>";
            body << "<td class=\"" << statusClass << "\">" << statusText << "</td>";
            body << "</tr>";
        }
        body << "</tbody></table>";
    }

    body << "</div>";
    return writeHtmlDocument(outputPath, "Reporte 1 - Historial de Pacientes", body.str());
}