#include "Hospital.h"

#include <sstream>
#include <unordered_map>

#include "Html.h"

bool writeHospitalStatsHtmlReport(
    const std::string& outputPath,
    const Hospital& data,
    const SemanticValidationResult& semanticValidation
) {
    std::unordered_map<std::string, int> pacientesPorSangre;
    std::unordered_map<std::string, int> medicosPorEspecialidad;

    for (std::size_t i = 0; i < data.pacientes.size(); ++i) {
        pacientesPorSangre[data.pacientes[i].tipoSangre]++;
    }

    for (std::size_t i = 0; i < data.medicos.size(); ++i) {
        medicosPorEspecialidad[data.medicos[i].especialidad]++;
    }

    int conflictCount = 0;
    for (std::size_t i = 0; i < semanticValidation.errors.size(); ++i) {
        if (semanticValidation.errors[i].type == "ConflictoHorario") {
            conflictCount++;
        }
    }

    std::ostringstream body;
    body << "<div class=\"grid cards\">";
    body << buildMetricCard("Pacientes", std::to_string(data.pacientes.size()));
    body << buildMetricCard("Medicos", std::to_string(data.medicos.size()));
    body << buildMetricCard("Citas", std::to_string(data.citas.size()));
    body << buildMetricCard("Diagnosticos", std::to_string(data.diagnosticos.size()));
    body << buildMetricCard("Conflictos de horario", std::to_string(conflictCount));
    body << buildMetricCard("Problemas semanticos totales", std::to_string(semanticValidation.errors.size()));
    body << "</div>";

    body << "<div class=\"section card\">";
    body << "<h2 class=\"section-title\">Resumen general</h2>";
    body << "<table><thead><tr><th>Indicador</th><th>Valor</th></tr></thead><tbody>";
    body << "<tr><td>Pacientes</td><td>" << data.pacientes.size() << "</td></tr>";
    body << "<tr><td>Medicos</td><td>" << data.medicos.size() << "</td></tr>";
    body << "<tr><td>Citas</td><td>" << data.citas.size() << "</td></tr>";
    body << "<tr><td>Diagnosticos</td><td>" << data.diagnosticos.size() << "</td></tr>";
    body << "<tr><td>Especialidades distintas</td><td>" << medicosPorEspecialidad.size() << "</td></tr>";
    body << "<tr><td>Tipos de sangre distintos</td><td>" << pacientesPorSangre.size() << "</td></tr>";
    body << "</tbody></table>";
    body << "</div>";

    body << "<div class=\"grid\" style=\"grid-template-columns: repeat(auto-fit, minmax(320px, 1fr)); margin-top: 18px;\">";
    body << "<div class=\"card\">";
    body << "<h2 class=\"section-title\">Pacientes por tipo de sangre</h2>";
    if (pacientesPorSangre.empty()) {
        body << "<p class=\"muted\">Sin pacientes cargados.</p>";
    } else {
        body << "<table><thead><tr><th>Tipo</th><th>Cantidad</th></tr></thead><tbody>";
        for (const auto& entry : pacientesPorSangre) {
            body << "<tr><td>" << htmlEscape(entry.first) << "</td><td>" << entry.second << "</td></tr>";
        }
        body << "</tbody></table>";
    }
    body << "</div>";

    body << "<div class=\"card\">";
    body << "<h2 class=\"section-title\">Medicos por especialidad</h2>";
    if (medicosPorEspecialidad.empty()) {
        body << "<p class=\"muted\">Sin medicos cargados.</p>";
    } else {
        body << "<table><thead><tr><th>Especialidad</th><th>Cantidad</th></tr></thead><tbody>";
        for (const auto& entry : medicosPorEspecialidad) {
            body << "<tr><td>" << htmlEscape(entry.first) << "</td><td>" << entry.second << "</td></tr>";
        }
        body << "</tbody></table>";
    }
    body << "</div>";
    body << "</div>";

    body << "<div class=\"section card\">";
    body << "<h2 class=\"section-title\">Estado semantico</h2>";
    if (semanticValidation.errors.empty()) {
        body << "<p class=\"ok\">Sin errores semanticos registrados.</p>";
    } else {
        body << "<p class=\"warn\">Se registraron " << semanticValidation.errors.size() << " incidencias semanticas.</p>";
        body << "<table><thead><tr><th>#</th><th>Tipo</th><th>Descripcion</th><th>Entidad</th><th>Linea</th></tr></thead><tbody>";
        for (std::size_t i = 0; i < semanticValidation.errors.size(); ++i) {
            const SemanticError& error = semanticValidation.errors[i];
            body << "<tr><td>" << error.number << "</td><td>" << htmlEscape(error.type) << "</td><td>" << htmlEscape(error.description) << "</td><td>" << htmlEscape(error.entity) << "</td><td>" << error.line << "</td></tr>";
        }
        body << "</tbody></table>";
    }
    body << "</div>";

    return writeHtmlDocument(outputPath, "Reporte 4 - Estadistico General del Hospital", body.str());
}