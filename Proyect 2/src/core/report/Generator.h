#ifndef GENERATOR_H
#define GENERATOR_H

#include <string>

#include "../model/HospitalModel.h"
#include "../semantic/SemanticAnalyzer.h"

class Generator {
public:
    static bool generateAllHtmlReports(
        const std::string& outputDirectory,
        const Hospital& data,
        const SemanticValidationResult& semanticValidation,
        const std::string& sourceFilePath
    );
};

#endif