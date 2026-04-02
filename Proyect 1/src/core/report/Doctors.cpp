#include "Doctors.h"

#include <sstream>
#include <unordered_map>
#include <vector>

#include "Html.h"

bool writeDoctorLoadHtmlReport(const std::string& outputPath, const Hospital& data) {
    std::unordered_map<std::string, std::vector<Medico> > medicosPorEspecialidad;

    for (std::size_t i = 0; i < data.medicos.size(); ++i) {
        medicosPorEspecialidad[data.medicos[i].especialidad].push_back(data.medicos[i]);
    }

    std::ostringstream body;
    body << "<div class=\"grid cards\">";
    body << buildMetricCard("Medicos registrados", std::to_string(data.medicos.size()));
    body << buildMetricCard("Especialidades distintas", std::to_string(medicosPorEspecialidad.size()));
    body << buildMetricCard("Promedio por especialidad", medicosPorEspecialidad.empty() ? 
    std::string("0") : std::to_string(data.medicos.size() / medicosPorEspecialidad.size()));
    body << "</div>";

    body << "<div class=\"section card\">";
    body << "<h2 class=\"section-title\">Carga por especialidad</h2>";
    if (medicosPorEspecialidad.empty()) {
        body << "<p class=\"muted\">No hay medicos cargados para agrupar por especialidad.</p>";
    } else {
        body << "<table><thead><tr><th>Especialidad</th><th>Medicos</th><th>Lista</th></tr></thead><tbody>";
        for (const auto& entry : medicosPorEspecialidad) {
            std::vector<std::string> nombres;
            for (std::size_t i = 0; i < entry.second.size(); ++i) {
                nombres.push_back(entry.second[i].nombre);
            }
            body << "<tr><td>" << htmlEscape(entry.first) << "</td><td>" << entry.second.size() << "</td><td>" << htmlEscape(joinList(nombres)) << "</td></tr>";
        }
        body << "</tbody></table>";
    }
    body << "</div>";

    body << "<div class=\"section card\">";
    body << "<h2 class=\"section-title\">Detalle de medicos</h2>";
    if (data.medicos.empty()) {
        body << "<p class=\"muted\">No hay medicos cargados.</p>";
    } else {
        body << "<table><thead><tr><th>#</th><th>Nombre</th><th>Especialidad</th><th>Codigo</th><th>Linea</th></tr></thead><tbody>";
        for (std::size_t i = 0; i < data.medicos.size(); ++i) {
            body << "<tr><td>" << (i + 1) << "</td><td>" << htmlEscape(data.medicos[i].nombre) << "</td><td>" << htmlEscape(data.medicos[i].especialidad) << "</td><td class=\"mono\">" << htmlEscape(data.medicos[i].codigo) << "</td><td>" << data.medicos[i].line << "</td></tr>";
        }
        body << "</tbody></table>";
    }
    body << "</div>";

    return writeHtmlDocument(outputPath, "Reporte 2 - Carga de Medicos por Especialidad", body.str());
}