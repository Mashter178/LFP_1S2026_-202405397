#ifndef PATIENTS_H
#define PATIENTS_H

#include <string>

#include "../model/HospitalModel.h"

bool writePatientHistoryHtmlReport(const std::string& outputPath, const Hospital& data);

#endif