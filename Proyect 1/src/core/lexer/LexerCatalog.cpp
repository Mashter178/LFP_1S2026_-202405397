#include "Lexer.h"

void Lexer::initializeCatalogs() {
    m_keywords["HOSPITAL"] = TokenType::Hospital;
    m_keywords["PACIENTES"] = TokenType::Pacientes;
    m_keywords["paciente"] = TokenType::Paciente;
    m_keywords["MEDICOS"] = TokenType::Medicos;
    m_keywords["medico"] = TokenType::Medico;
    m_keywords["CITAS"] = TokenType::Citas;
    m_keywords["cita"] = TokenType::Cita;
    m_keywords["DIAGNOSTICOS"] = TokenType::Diagnosticos;
    m_keywords["diagnostico"] = TokenType::Diagnostico;
    m_keywords["especialidad"] = TokenType::Especialidad;
    m_keywords["fecha"] = TokenType::Fecha;
    m_keywords["hora"] = TokenType::Hora;
    m_keywords["dosis"] = TokenType::Dosis;
    m_keywords["tipo_sangre"] = TokenType::Tipo_Sangre;
    m_keywords["codigo"] = TokenType::Codigo;
    m_keywords["condicion"] = TokenType::Condicion;
    m_keywords["medicamento"] = TokenType::Medicamento;
    m_keywords["edad"] = TokenType::Edad;
    m_keywords["habitacion"] = TokenType::Habitacion;
    m_keywords["con"] = TokenType::Con;

    m_specialties.insert("CARDIOLOGIA");
    m_specialties.insert("NEUROLOGIA");
    m_specialties.insert("PEDIATRIA");
    m_specialties.insert("CIRUGIA");
    m_specialties.insert("MEDICINA_GENERAL");
    m_specialties.insert("ONCOLOGIA");

    m_doseFrequencies.insert("DIARIA");
    m_doseFrequencies.insert("CADA_8_HORAS");
    m_doseFrequencies.insert("CADA_12_HORAS");
    m_doseFrequencies.insert("SEMANAL");

    m_bloodTypes.insert("A+");
    m_bloodTypes.insert("A-");
    m_bloodTypes.insert("B+");
    m_bloodTypes.insert("B-");
    m_bloodTypes.insert("O+");
    m_bloodTypes.insert("O-");
    m_bloodTypes.insert("AB+");
    m_bloodTypes.insert("AB-");

    for (std::unordered_map<std::string, TokenType>::const_iterator it = m_keywords.begin(); it != m_keywords.end(); ++it) {
        m_reservedLexemes.insert(it->first);
    }
    for (std::unordered_set<std::string>::const_iterator it = m_specialties.begin(); it != m_specialties.end(); ++it) {
        m_reservedLexemes.insert(*it);
    }
    for (std::unordered_set<std::string>::const_iterator it = m_doseFrequencies.begin(); it != m_doseFrequencies.end(); ++it) {
        m_reservedLexemes.insert(*it);
    }
}
