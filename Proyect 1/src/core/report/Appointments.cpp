#include "Appointments.h"

#include <sstream>
#include <unordered_map>
#include <vector>

#include "Html.h"

bool writeAppointmentsAgendaHtmlReport(
    const std::string& outputPath,
    const Hospital& data,
    const SemanticValidationResult& semanticValidation
) {
    std::unordered_map<std::string, int> citaCounts;
    for (std::size_t i = 0; i < data.citas.size(); ++i) {
        const Cita& cita = data.citas[i];
        citaCounts[cita.medico + "|" + cita.fecha + "|" + cita.hora]++;
    }

    std::vector<SemanticError> conflicts;
    for (std::size_t i = 0; i < semanticValidation.errors.size(); ++i) {
        if (semanticValidation.errors[i].type == "ConflictoHorario") {
            conflicts.push_back(semanticValidation.errors[i]);
        }
    }

    std::ostringstream body;
    body << "<div class=\"grid cards\">";
    body << buildMetricCard("Citas registradas", std::to_string(data.citas.size()));
    body << buildMetricCard("Conflictos detectados", std::to_string(conflicts.size()), conflicts.empty() ? "Sin choques de horario." : "Revisar filas marcadas como conflicto.");
    body << buildMetricCard("Citas unicas por medico-fecha-hora", std::to_string(citaCounts.size()));
    body << "</div>";

    body << "<div class=\"section card\">";
    body << "<h2 class=\"section-title\">Agenda de citas</h2>";
    if (data.citas.empty()) {
        body << "<p class=\"muted\">No hay citas cargadas.</p>";
    } else {
        body << "<table><thead><tr><th>#</th><th>Paciente</th><th>Medico</th><th>Fecha</th><th>Hora</th><th>Estado</th><th>Linea</th></tr></thead><tbody>";
        for (std::size_t i = 0; i < data.citas.size(); ++i) {
            const Cita& cita = data.citas[i];
            const std::string key = cita.medico + "|" + cita.fecha + "|" + cita.hora;
            const bool conflict = citaCounts[key] > 1;
            body << "<tr>";
            body << "<td>" << (i + 1) << "</td>";
            body << "<td>" << htmlEscape(cita.paciente) << "</td>";
            body << "<td>" << htmlEscape(cita.medico) << "</td>";
            body << "<td>" << htmlEscape(cita.fecha) << "</td>";
            body << "<td>" << htmlEscape(cita.hora) << "</td>";
            if (conflict) {
                body << "<td><span class=\"pill danger\">Conflicto</span></td>";
            } else {
                body << "<td><span class=\"pill ok\">Libre</span></td>";
            }
            body << "<td>" << cita.line << "</td>";
            body << "</tr>";
        }
        body << "</tbody></table>";
    }
    body << "</div>";

    body << "<div class=\"section card\">";
    body << "<h2 class=\"section-title\">Conflictos de horario</h2>";
    if (conflicts.empty()) {
        body << "<p class=\"ok\">No se detectaron conflictos de horario.</p>";
    } else {
        body << "<table><thead><tr><th>#</th><th>Tipo</th><th>Descripcion</th><th>Entidad</th><th>Linea</th></tr></thead><tbody>";
        for (std::size_t i = 0; i < conflicts.size(); ++i) {
            const SemanticError& error = conflicts[i];
            body << "<tr><td>" << error.number << "</td><td>" << htmlEscape(error.type) << "</td><td>" << htmlEscape(error.description) << "</td><td>" << htmlEscape(error.entity) << "</td><td>" << error.line << "</td></tr>";
        }
        body << "</tbody></table>";
    }
    body << "</div>";

    return writeHtmlDocument(outputPath, "Reporte 3 - Agenda de Citas con Conflictos", body.str());
}