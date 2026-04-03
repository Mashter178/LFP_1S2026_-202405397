#include "Generator.h"

#include <filesystem>
#include <sstream>
#include <system_error>

#include "Appointments.h"
#include "Doctors.h"
#include "Graphviz.h"
#include "Hospital.h"
#include "Html.h"
#include "Patients.h"

namespace {

std::string buildReportsIndexBody() {
    std::ostringstream body;
    body << "<div class=\"card\">";
    body << "<h2 class=\"section-title\">Centro de reportes</h2>";
    body << "<p class=\"muted\">Selecciona un reporte para abrirlo en el navegador.</p>";
    body << "<table><thead><tr><th>#</th><th>Reporte</th><th>Archivo</th></tr></thead><tbody>";
    body << "<tr><td>1</td><td>Reporte Lexico</td><td><a href=\"reporte_lexico.html\">reporte_lexico.html</a></td></tr>";
    body << "<tr><td>2</td><td>Historial de Pacientes</td><td><a href=\"historial_pacientes.html\">historial_pacientes.html</a></td></tr>";
    body << "<tr><td>3</td><td>Carga de Medicos por Especialidad</td><td><a href=\"carga_medicos_especialidad.html\">carga_medicos_especialidad.html</a></td></tr>";
    body << "<tr><td>4</td><td>Agenda de Citas con Conflictos</td><td><a href=\"agenda_citas_conflictos.html\">agenda_citas_conflictos.html</a></td></tr>";
    body << "<tr><td>5</td><td>Estadistico General del Hospital</td><td><a href=\"estadistico_general_hospital.html\">estadistico_general_hospital.html</a></td></tr>";
    body << "<tr><td>6</td><td>Diagrama Graphviz</td><td><a href=\"hospital.dot\">hospital.dot</a></td></tr>";
    body << "</tbody></table>";
    body << "</div>";
    return body.str();
}

} // namespace

bool Generator::generateAllHtmlReports(
    const std::string& outputDirectory,
    const Hospital& data,
    const SemanticValidationResult& semanticValidation,
    const std::string& sourceFilePath
) {
    const std::filesystem::path basePath(outputDirectory);
    std::error_code errorCode;
    std::filesystem::create_directories(basePath, errorCode);

    bool ok = true;
    ok = writePatientHistoryHtmlReport((basePath / "historial_pacientes.html").string(), data) && ok;
    ok = writeDoctorLoadHtmlReport((basePath / "carga_medicos_especialidad.html").string(), data) && ok;
    ok = writeAppointmentsAgendaHtmlReport((basePath / "agenda_citas_conflictos.html").string(), data, semanticValidation) && ok;
    ok = writeHospitalStatsHtmlReport((basePath / "estadistico_general_hospital.html").string(), data, semanticValidation, sourceFilePath) && ok;
    ok = writeHospitalGraphvizDot((basePath / "hospital.dot").string(), data, sourceFilePath) && ok;
    ok = writeHtmlDocument((basePath / "indice_reportes.html").string(), "Indice de Reportes MedLang", buildReportsIndexBody()) && ok;

    return ok;
}