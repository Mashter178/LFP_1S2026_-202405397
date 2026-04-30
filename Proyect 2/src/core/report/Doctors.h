#ifndef DOCTORS_H
#define DOCTORS_H

#include <string>

#include "../model/HospitalModel.h"

bool writeDoctorLoadHtmlReport(const std::string& outputPath, const Hospital& data);

#endif