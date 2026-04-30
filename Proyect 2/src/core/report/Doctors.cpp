#include "Doctors.h"

#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "Html.h"

namespace {

struct DoctorLoadInfo {
    int totalAppointments;
    int uniquePatients;
    std::string level;
    std::string levelClass;
};

DoctorLoadInfo calculateDoctorLoad(int totalAppointments, int uniquePatients) {
    (void)uniquePatients;
    if (totalAppointments <= 2) {
        return {totalAppointments, uniquePatients, "BAJA", "info"};
    }
    if (totalAppointments <= 5) {
        return {totalAppointments, uniquePatients, "NORMAL", "ok"};
    }
    if (totalAppointments <= 8) {
        return {totalAppointments, uniquePatients, "ALTA", "warn"};
    }
    return {totalAppointments, uniquePatients, "SATURADA", "danger"};
}

}

bool writeDoctorLoadHtmlReport(const std::string& outputPath, const Hospital& data) {
    std::unordered_map<std::string, int> citasPorMedico;
    std::unordered_map<std::string, std::set<std::string> > pacientesUnicosPorMedico;

    for (std::size_t i = 0; i < data.citas.size(); ++i) {
        const Cita& cita = data.citas[i];
        citasPorMedico[cita.medico]++;
        pacientesUnicosPorMedico[cita.medico].insert(cita.paciente);
    }

    std::ostringstream body;
    body << "<div class=\"grid cards\">";
    body << buildMetricCard("Medicos registrados", std::to_string(data.medicos.size()));
    body << buildMetricCard("Citas programadas", std::to_string(data.citas.size()));
    body << buildMetricCard("Medicos con al menos una cita", std::to_string(citasPorMedico.size()));
    body << "</div>";

    body << "<div class=\"section card\">";
    body << "<h2 class=\"section-title\">Carga de medicos por especialidad</h2>";
    if (data.medicos.empty()) {
        body << "<p class=\"muted\">No hay medicos cargados.</p>";
    } else {
        body << "<table><thead><tr><th>Medico</th><th>Codigo</th><th>Especialidad</th><th>Citas Prog.</th><th>Pacientes</th><th>Nivel de Carga</th></tr></thead><tbody>";
        for (std::size_t i = 0; i < data.medicos.size(); ++i) {
            const Medico& medico = data.medicos[i];
            const int totalAppointments = citasPorMedico[medico.nombre];
            const int uniquePatients = static_cast<int>(pacientesUnicosPorMedico[medico.nombre].size());
            const DoctorLoadInfo loadInfo = calculateDoctorLoad(totalAppointments, uniquePatients);

            std::string levelCellClass = "status-info";
            if (loadInfo.levelClass == "ok") {
                levelCellClass = "status-ok";
            } else if (loadInfo.levelClass == "warn") {
                levelCellClass = "status-warn";
            } else if (loadInfo.levelClass == "danger") {
                levelCellClass = "status-danger";
            }

            body << "<tr>";
            body << "<td>" << htmlEscape(medico.nombre) << "</td>";
            body << "<td class=\"mono\">" << htmlEscape(medico.codigo) << "</td>";
            body << "<td>" << htmlEscape(medico.especialidad) << "</td>";
            body << "<td>" << loadInfo.totalAppointments << "</td>";
            body << "<td>" << loadInfo.uniquePatients << "</td>";
            body << "<td class=\"" << levelCellClass << "\">" << loadInfo.level << "</td>";
            body << "</tr>";
        }
        body << "</tbody></table>";
    }
    body << "</div>";

    return writeHtmlDocument(outputPath, "Reporte 2 - Carga de Medicos por Especialidad", body.str());
}