#ifndef HOSPITAL_MODEL_H
#define HOSPITAL_MODEL_H

#include <string>
#include <vector>

struct PacienteData {
    std::string nombre;
    int edad;
    std::string tipoSangre;
    int habitacion;
    int line;
};

struct MedicoData {
    std::string nombre;
    std::string especialidad;
    std::string codigo;
    int line;
};

struct CitaData {
    std::string paciente;
    std::string medico;
    std::string fecha;
    std::string hora;
    int line;
};

struct DiagnosticoData {
    std::string paciente;
    std::string condicion;
    std::string medicamento;
    std::string dosis;
    int line;
};

struct HospitalData {
    std::vector<PacienteData> pacientes;
    std::vector<MedicoData> medicos;
    std::vector<CitaData> citas;
    std::vector<DiagnosticoData> diagnosticos;
};

#endif
