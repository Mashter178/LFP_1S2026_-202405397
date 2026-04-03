#include "Appointments.h"

#include <ctime>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "Html.h"

namespace {

bool parseDate(const std::string& dateText, int& year, int& month, int& day) {
    if (dateText.size() != 10 || dateText[4] != '-' || dateText[7] != '-') {
        return false;
    }

    try {
        year = std::stoi(dateText.substr(0, 4));
        month = std::stoi(dateText.substr(5, 2));
        day = std::stoi(dateText.substr(8, 2));
    } catch (...) {
        return false;
    }

    return true;
}

bool isFutureDate(const std::string& dateText) {
    int year = 0;
    int month = 0;
    int day = 0;
    if (!parseDate(dateText, year, month, day)) {
        return false;
    }

    std::time_t now = std::time(nullptr);
    std::tm today = *std::localtime(&now);

    const int todayYear = today.tm_year + 1900;
    const int todayMonth = today.tm_mon + 1;
    const int todayDay = today.tm_mday;

    if (year != todayYear) {
        return year > todayYear;
    }
    if (month != todayMonth) {
        return month > todayMonth;
    }
    return day > todayDay;
}

}

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

    std::unordered_map<std::string, std::string> specialtyByDoctor;
    for (std::size_t i = 0; i < data.medicos.size(); ++i) {
        specialtyByDoctor[data.medicos[i].nombre] = data.medicos[i].especialidad;
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
        body << "<table><thead><tr><th>Fecha</th><th>Hora</th><th>Paciente</th><th>Medico</th><th>Especialidad</th><th>Estado</th></tr></thead><tbody>";
        for (std::size_t i = 0; i < data.citas.size(); ++i) {
            const Cita& cita = data.citas[i];
            const std::string key = cita.medico + "|" + cita.fecha + "|" + cita.hora;
            const bool conflict = citaCounts[key] > 1;
            const bool pending = !conflict && isFutureDate(cita.fecha);

            if (conflict) {
                body << "<tr style=\"background:#fee2e2;\">";
            } else if (pending) {
                body << "<tr style=\"background:#ffedd5;\">";
            } else {
                body << "<tr style=\"background:#dcfce7;\">";
            }

            body << "<td>" << htmlEscape(cita.fecha) << "</td>";
            body << "<td>" << htmlEscape(cita.hora) << "</td>";
            body << "<td>" << htmlEscape(cita.paciente) << "</td>";
            body << "<td>" << htmlEscape(cita.medico) << "</td>";

            const auto specialtyIt = specialtyByDoctor.find(cita.medico);
            const std::string specialty = specialtyIt == specialtyByDoctor.end() ? "No definida" : specialtyIt->second;
            body << "<td>" << htmlEscape(specialty) << "</td>";

            if (conflict) {
                body << "<td class=\"status-danger\">⚠ CONFLICTO</td>";
            } else if (pending) {
                body << "<td class=\"status-warn\">PENDIENTE</td>";
            } else {
                body << "<td class=\"status-ok\">CONFIRMADA</td>";
            }
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