#ifndef APPOINTMENTS_H
#define APPOINTMENTS_H

#include <string>

#include "../model/HospitalModel.h"
#include "../semantic/SemanticAnalyzer.h"

bool writeAppointmentsAgendaHtmlReport(
    const std::string& outputPath,
    const Hospital& data,
    const SemanticValidationResult& semanticValidation
);

#endif