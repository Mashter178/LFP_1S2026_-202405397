#ifndef GRAPHVIZ_REPORT_H
#define GRAPHVIZ_REPORT_H

#include <string>

#include "../model/HospitalModel.h"

bool writeHospitalGraphvizDot(
    const std::string& outputPath,
    const Hospital& data,
    const std::string& sourceFilePath
);

#endif
