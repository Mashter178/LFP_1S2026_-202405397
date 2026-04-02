#ifndef HOSPITAL_MODEL_H
#define HOSPITAL_MODEL_H

#include <string>
#include <vector>

struct Paciente {
    std::string nombre;
    int edad;
    std::string tipoSangre;
    int habitacion;
    int line;
};

struct Medico {
    std::string nombre;
    std::string especialidad;
    std::string codigo;
    int line;
};

struct Cita {
    std::string paciente;
    std::string medico;
    std::string fecha;
    std::string hora;
    int line;
};

struct Diagnostico {
    std::string paciente;
    std::string condicion;
    std::string medicamento;
    std::string dosis;
    int line;
};

struct Hospital {
    std::vector<Paciente> pacientes;
    std::vector<Medico> medicos;
    std::vector<Cita> citas;
    std::vector<Diagnostico> diagnosticos;
};

#endif
