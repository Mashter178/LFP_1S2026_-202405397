#include "Hospital.h"

#include <filesystem>
#include <iomanip>
#include <set>
#include <sstream>
#include <unordered_map>

#include "Html.h"

bool writeHospitalStatsHtmlReport(
    const std::string& outputPath,
    const Hospital& data,
    const SemanticValidationResult& semanticValidation,
    const std::string& sourceFilePath
) {
    std::unordered_map<std::string, std::set<std::string> > pacientesPorMedicamento;
    std::unordered_map<std::string, int> frecuenciaMedicamento;
    std::unordered_map<std::string, std::string> especialidadPorMedico;
    std::unordered_map<std::string, int> citasPorEspecialidad;
    std::unordered_map<std::string, int> citasPorMedico;

    int sumEdades = 0;
    for (std::size_t i = 0; i < data.pacientes.size(); ++i) {
        sumEdades += data.pacientes[i].edad;
    }

    for (std::size_t i = 0; i < data.medicos.size(); ++i) {
        especialidadPorMedico[data.medicos[i].nombre] = data.medicos[i].especialidad;
    }

    for (std::size_t i = 0; i < data.diagnosticos.size(); ++i) {
        const Diagnostico& diagnostico = data.diagnosticos[i];
        frecuenciaMedicamento[diagnostico.medicamento]++;
        pacientesPorMedicamento[diagnostico.medicamento].insert(diagnostico.paciente);
    }

    for (std::size_t i = 0; i < data.citas.size(); ++i) {
        const Cita& cita = data.citas[i];
        const std::string especialidad = especialidadPorMedico[cita.medico];
        citasPorEspecialidad[especialidad]++;
        citasPorMedico[cita.medico]++;
    }

    std::string medicamentoTop = "N/A";
    int medicamentoTopFrecuencia = 0;
    int medicamentoTopPacientes = 0;
    for (const auto& entry : frecuenciaMedicamento) {
        if (entry.second > medicamentoTopFrecuencia) {
            medicamentoTop = entry.first;
            medicamentoTopFrecuencia = entry.second;
            medicamentoTopPacientes = static_cast<int>(pacientesPorMedicamento[entry.first].size());
        }
    }

    int conflictCount = 0;
    for (std::size_t i = 0; i < semanticValidation.errors.size(); ++i) {
        if (semanticValidation.errors[i].type == "ConflictoHorario") {
            conflictCount++;
        }
    }

    std::set<std::string> pacientesConDiagnostico;
    for (std::size_t i = 0; i < data.diagnosticos.size(); ++i) {
        pacientesConDiagnostico.insert(data.diagnosticos[i].paciente);
    }

    const double porcentajeActivos = data.pacientes.empty()
        ? 0.0
        : (100.0 * static_cast<double>(pacientesConDiagnostico.size()) / static_cast<double>(data.pacientes.size()));

    std::string especialidadTop = "N/A";
    int especialidadTopCitas = 0;
    for (const auto& entry : citasPorEspecialidad) {
        if (entry.second > especialidadTopCitas) {
            especialidadTop = entry.first;
            especialidadTopCitas = entry.second;
        }
    }

    std::string medicoTop = "N/A";
    int medicoTopCitas = 0;
    for (const auto& entry : citasPorMedico) {
        const std::string especialidad = especialidadPorMedico[entry.first];
        if (especialidad == especialidadTop && entry.second > medicoTopCitas) {
            medicoTop = entry.first;
            medicoTopCitas = entry.second;
        }
    }

    const double promedioEdad = data.pacientes.empty()
        ? 0.0
        : static_cast<double>(sumEdades) / static_cast<double>(data.pacientes.size());

    std::ostringstream porcentajeActivoText;
    porcentajeActivoText << std::fixed << std::setprecision(1) << porcentajeActivos;

    std::ostringstream promedioEdadText;
    promedioEdadText << std::fixed << std::setprecision(1) << promedioEdad;

    const std::string hospitalName = std::filesystem::path(sourceFilePath).filename().string();

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
    body << "<tr><td>Nombre del hospital</td><td>" << htmlEscape(hospitalName.empty() ? std::string("No definido") : hospitalName) << "</td></tr>";
    body << "<tr><td>Total de pacientes registrados</td><td>" << data.pacientes.size() << "</td></tr>";
    body << "<tr><td>Total de medicos activos</td><td>" << data.medicos.size() << "</td></tr>";
    body << "<tr><td>Total de citas programadas</td><td>" << data.citas.size() << "</td></tr>";
    body << "<tr><td>Citas con conflicto de horario</td><td>" << conflictCount << "</td></tr>";
    body << "<tr><td>Pacientes con diagnostico activo</td><td>" << pacientesConDiagnostico.size() << " / " << data.pacientes.size() << " (" << porcentajeActivoText.str() << "%)</td></tr>";
    body << "<tr><td>Medicamento mas prescrito</td><td>" << htmlEscape(medicamentoTop) << " (" << medicamentoTopPacientes << " pacientes)</td></tr>";
    body << "<tr><td>Especialidad con mayor carga de citas</td><td>" << htmlEscape(especialidadTop) << " — " << htmlEscape(medicoTop) << " (" << medicoTopCitas << " citas)</td></tr>";
    body << "<tr><td>Promedio de edad de los pacientes</td><td>" << promedioEdadText.str() << " años</td></tr>";
    body << "</tbody></table>";
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