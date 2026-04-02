#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <string>

#include "../model/HospitalModel.h"
#include "../semantic/SemanticAnalyzer.h"

bool writeHospitalStatsHtmlReport(
    const std::string& outputPath,
    const Hospital& data,
    const SemanticValidationResult& semanticValidation
);

#endif