#include "Patients.h"

#include <sstream>
#include <unordered_map>
#include <vector>

#include "Html.h"

bool writePatientHistoryHtmlReport(const std::string& outputPath, const Hospital& data) {
    std::unordered_map<std::string, std::vector<Cita> > citasPorPaciente;
    std::unordered_map<std::string, std::vector<Diagnostico> > diagnosticosPorPaciente;

    for (std::size_t i = 0; i < data.citas.size(); ++i) {
        citasPorPaciente[data.citas[i].paciente].push_back(data.citas[i]);
    }

    for (std::size_t i = 0; i < data.diagnosticos.size(); ++i) {
        diagnosticosPorPaciente[data.diagnosticos[i].paciente].push_back(data.diagnosticos[i]);
    }

    std::ostringstream body;
    body << "<div class=\"grid cards\">";
    body << buildMetricCard("Pacientes registrados", std::to_string(data.pacientes.size()));
    body << buildMetricCard("Citas ligadas a pacientes", std::to_string(data.citas.size()));
    body << buildMetricCard("Diagnosticos ligados a pacientes", std::to_string(data.diagnosticos.size()));
    body << "</div>";

    body << "<div class=\"section card\">";
    body << "<h2 class=\"section-title\">Historial por paciente</h2>";

    if (data.pacientes.empty()) {
        body << "<p class=\"muted\">No hay pacientes cargados para construir el historial.</p>";
    } else {
        for (std::size_t i = 0; i < data.pacientes.size(); ++i) {
            const Paciente& paciente = data.pacientes[i];
            const std::vector<Cita>& citas = citasPorPaciente[paciente.nombre];
            const std::vector<Diagnostico>& diagnosticos = diagnosticosPorPaciente[paciente.nombre];

            body << "<div class=\"patient-block\">";
            body << "<h3>" << htmlEscape(paciente.nombre) << "</h3>";
            body << "<div class=\"grid cards\">";
            body << buildMetricCard("Edad", std::to_string(paciente.edad));
            body << buildMetricCard("Tipo de sangre", paciente.tipoSangre);
            body << buildMetricCard("Habitacion", std::to_string(paciente.habitacion));
            body << buildMetricCard("Citas", std::to_string(citas.size()));
            body << buildMetricCard("Diagnosticos", std::to_string(diagnosticos.size()));
            body << "</div>";

            body << "<div class=\"section subtle\">";
            body << "<h4 class=\"section-title\">Citas</h4>";
            if (citas.empty()) {
                body << "<p class=\"muted\">Sin citas registradas.</p>";
            } else {
                body << "<table><thead><tr><th>#</th><th>Medico</th><th>Fecha</th><th>Hora</th><th>Linea</th></tr></thead><tbody>";
                for (std::size_t j = 0; j < citas.size(); ++j) {
                    body << "<tr><td>" << (j + 1) << "</td><td>" << htmlEscape(citas[j].medico) << "</td><td>" << htmlEscape(citas[j].fecha) << "</td><td>" << htmlEscape(citas[j].hora) << "</td><td>" << citas[j].line << "</td></tr>";
                }
                body << "</tbody></table>";
            }
            body << "</div>";

            body << "<div class=\"section subtle\">";
            body << "<h4 class=\"section-title\">Diagnosticos</h4>";
            if (diagnosticos.empty()) {
                body << "<p class=\"muted\">Sin diagnosticos registrados.</p>";
            } else {
                body << "<table><thead><tr><th>#</th><th>Condicion</th><th>Medicamento</th><th>Dosis</th><th>Linea</th></tr></thead><tbody>";
                for (std::size_t j = 0; j < diagnosticos.size(); ++j) {
                    body << "<tr><td>" << (j + 1) << "</td><td>" << htmlEscape(diagnosticos[j].condicion) << "</td><td>" << htmlEscape(diagnosticos[j].medicamento) << "</td><td>" << htmlEscape(diagnosticos[j].dosis) << "</td><td>" << diagnosticos[j].line << "</td></tr>";
                }
                body << "</tbody></table>";
            }
            body << "</div>";
            body << "</div>";
        }
    }

    body << "</div>";
    return writeHtmlDocument(outputPath, "Reporte 1 - Historial de Pacientes", body.str());
}