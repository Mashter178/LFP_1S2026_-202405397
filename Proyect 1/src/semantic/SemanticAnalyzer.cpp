#include "semantic/SemanticAnalyzer.h"

#include <unordered_map>
#include <unordered_set>

SemanticAnalyzer::SemanticAnalyzer(const HospitalData& data)
    : m_data(data) {}

SemanticRecognitionResult SemanticAnalyzer::recognizeInput() const {
    SemanticRecognitionResult result;
    result.inputReady = true;

    result.notes.push_back("Entrada semantica reconocida desde parser.");
    result.notes.push_back("Pacientes cargados: " + std::to_string(m_data.pacientes.size()));
    result.notes.push_back("Medicos cargados: " + std::to_string(m_data.medicos.size()));
    result.notes.push_back("Citas cargadas: " + std::to_string(m_data.citas.size()));
    result.notes.push_back("Diagnosticos cargados: " + std::to_string(m_data.diagnosticos.size()));

    if (m_data.pacientes.empty()) {
        result.inputReady = false;
        result.notes.push_back("Seccion PACIENTES vacia.");
    }
    if (m_data.medicos.empty()) {
        result.inputReady = false;
        result.notes.push_back("Seccion MEDICOS vacia.");
    }
    if (m_data.citas.empty()) {
        result.inputReady = false;
        result.notes.push_back("Seccion CITAS vacia.");
    }
    if (m_data.diagnosticos.empty()) {
        result.inputReady = false;
        result.notes.push_back("Seccion DIAGNOSTICOS vacia.");
    }

    return result;
}

SemanticValidationResult SemanticAnalyzer::validateBasicRules() const {
    SemanticValidationResult result;
    result.valid = true;

    std::unordered_set<std::string> pacientes;
    std::unordered_set<std::string> medicos;
    std::unordered_map<std::string, int> firstScheduleLine;
    int errorCounter = 0;

    for (std::size_t i = 0; i < m_data.pacientes.size(); ++i) {
        pacientes.insert(m_data.pacientes[i].nombre);
    }

    for (std::size_t i = 0; i < m_data.medicos.size(); ++i) {
        medicos.insert(m_data.medicos[i].nombre);
    }

    for (std::size_t i = 0; i < m_data.citas.size(); ++i) {
        const CitaData& cita = m_data.citas[i];

        if (pacientes.find(cita.paciente) == pacientes.end()) {
            SemanticError err;
            err.number = ++errorCounter;
            err.type = "PacienteInexistente";
            err.description = "La cita referencia un paciente no registrado";
            err.entity = cita.paciente;
            err.line = cita.line;
            result.errors.push_back(err);
            result.valid = false;
        }

        if (medicos.find(cita.medico) == medicos.end()) {
            SemanticError err;
            err.number = ++errorCounter;
            err.type = "MedicoInexistente";
            err.description = "La cita referencia un medico no registrado";
            err.entity = cita.medico;
            err.line = cita.line;
            result.errors.push_back(err);
            result.valid = false;
        }

        const std::string scheduleKey = cita.medico + "|" + cita.fecha + "|" + cita.hora;
        const auto it = firstScheduleLine.find(scheduleKey);
        if (it == firstScheduleLine.end()) {
            firstScheduleLine[scheduleKey] = cita.line;
        } else {
            SemanticError err;
            err.number = ++errorCounter;
            err.type = "ConflictoHorario";
            err.description = "Conflicto de horario para medico en la misma fecha y hora (primera ocurrencia en linea " + std::to_string(it->second) + ")";
            err.entity = cita.medico + " @ " + cita.fecha + " " + cita.hora;
            err.line = cita.line;
            result.errors.push_back(err);
            result.valid = false;
        }
    }

    return result;
}
