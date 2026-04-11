# Salidas Esperadas para Archivos de Prueba MedLexer

---

## ARCHIVO 1: test_basico.med

### Errores Léxicos Esperados
**Ningún error.** El archivo es completamente válido.

### Tabla de Tokens (resumen de tipos esperados)
- Palabras reservadas: HOSPITAL, PACIENTES, paciente (×3), MEDICOS, medico (×2), CITAS, cita (×3), DIAGNOSTICOS, diagnostico (×2), con (×3)
- Palabras reservadas de atributo: edad (×3), tipo_sangre (×3), habitacion (×3), especialidad (×2), codigo (×2), fecha (×3), hora (×3), condicion (×2), medicamento (×2), dosis (×2)
- Cadenas: "Ana López", "Juan Pérez", "Laura Díaz", "Dr. Carlos Ruiz", "Dra. María Solís", "O+", "B-", "A+", "MED-001", "MED-002", "Hipertensión Arterial", "Losartán 50mg", "Migraña Crónica", "Topiramato 25mg"
- Enteros: 34, 50, 28, 101, 205, 310
- Fechas: 2025-04-10, 2025-04-11, 2025-04-12
- Horas: 09:00, 14:30, 10:00
- Enumeraciones de especialidad: CARDIOLOGIA, NEUROLOGIA
- Enumeraciones de dosis: DIARIA, CADA_12_HORAS
- Delimitadores: { (×5), } (×5), ; (×5), [ (×10), ] (×10), , (múltiples), : (múltiples)

### Reporte 1 — Historial de Pacientes

| Paciente     | Edad | Sangre | Diagnóstico activo      | Medicamento / Dosis          | Estado    |
|--------------|------|--------|-------------------------|------------------------------|-----------|
| Ana López    | 34   | O+     | Hipertensión Arterial   | Losartán 50mg / DIARIA       | ACTIVO    |
| Juan Pérez   | 50   | B-     | Migraña Crónica         | Topiramato 25mg / CADA_12_HORAS | ACTIVO |
| Laura Díaz   | 28   | A+     | Sin diagnóstico registrado | —                         | SIN DIAG. |

### Reporte 2 — Carga de Médicos por Especialidad

| Médico            | Código  | Especialidad | Citas Prog. | Pacientes | Nivel de Carga |
|-------------------|---------|-------------|-------------|-----------|----------------|
| Dr. Carlos Ruiz   | MED-001 | CARDIOLOGIA | 2           | 2         | BAJA           |
| Dra. María Solís  | MED-002 | NEUROLOGIA  | 1           | 1         | BAJA           |

### Reporte 3 — Agenda de Citas

| Fecha      | Hora  | Paciente     | Médico            | Especialidad | Estado      |
|------------|-------|--------------|-------------------|-------------|-------------|
| 2025-04-10 | 09:00 | Ana López    | Dr. Carlos Ruiz   | CARDIOLOGIA | CONFIRMADA  |
| 2025-04-11 | 14:30 | Juan Pérez   | Dra. María Solís  | NEUROLOGIA  | CONFIRMADA  |
| 2025-04-12 | 10:00 | Laura Díaz   | Dr. Carlos Ruiz   | CARDIOLOGIA | CONFIRMADA  |

**Conflictos detectados: 0**

### Reporte 4 — Estadístico General

| Indicador                              | Valor                                        |
|----------------------------------------|----------------------------------------------|
| Total de pacientes registrados         | 3                                            |
| Total de médicos activos               | 2                                            |
| Total de citas programadas             | 3                                            |
| Citas con conflicto de horario         | 0                                            |
| Pacientes con diagnóstico activo       | 2 de 3 (66.7%)                               |
| Medicamento más prescrito              | Losartán 50mg (1) / Topiramato 25mg (1)      |
| Especialidad con mayor carga de citas  | CARDIOLOGIA — Dr. Carlos Ruiz (2 citas)      |
| Promedio de edad de los pacientes      | 37.3 años                                    |

---

## ARCHIVO 2: test_medio.med

### Errores Léxicos Esperados

| No. | Lexema     | Tipo de Error              | Descripción                                                                 | Línea | Columna | Gravedad |
|-----|------------|---------------------------|-----------------------------------------------------------------------------|-------|---------|----------|
| 1   | "O++"      | Tipo de sangre inválido   | El valor 'O++' no es un tipo de sangre reconocido. Valores válidos: O+, O-, A+, A-, B+, B-, AB+, AB-. | 7 | 53 | ERROR |
| 2   | DERMATOLOGIA | Especialidad no reconocida | 'DERMATOLOGIA' no es una especialidad válida. Use: CARDIOLOGIA, NEUROLOGIA, PEDIATRIA, CIRUGIA, MEDICINA_GENERAL u ONCOLOGIA. | 14 | 50 | ERROR |
| 3   | 2025-13-05 | Fecha con mes inválido    | El mes '13' está fuera del rango permitido (01-12). Formato esperado: AAAA-MM-DD. | 20 | 56 | ERROR |
| 4   | 25:00      | Hora fuera de rango       | La hora '25:00' está fuera del rango permitido (00:00 - 23:59). Formato esperado: HH:MM en 24 horas. | 21 | 62 | ERROR |

### Reporte 1 — Historial de Pacientes

| Paciente         | Edad | Sangre | Diagnóstico activo       | Medicamento / Dosis                | Estado    |
|------------------|------|--------|--------------------------|------------------------------------|-----------|
| María García     | 45   | A+     | Hipertensión Arterial    | Losartán 50mg / DIARIA             | ACTIVO    |
| Carlos Mendoza   | 62   | O-     | Diabetes Tipo 2          | Metformina 850mg / CADA_12_HORAS   | ACTIVO    |
| Sofía Ramírez    | 29   | B+     | Sin diagnóstico registrado | —                                | SIN DIAG. |
| Pedro Castillo   | 71   | AB-    | Insuficiencia Cardiaca   | Furosemida 40mg / DIARIA           | ACTIVO    |
| Elena Morales    | 38   | (error)| Sin diagnóstico registrado | —                                | SIN DIAG. |
| Roberto Flores   | 55   | A-     | Arritmia Ventricular     | Amiodarona 200mg / SEMANAL         | ACTIVO    |

### Reporte 2 — Carga de Médicos por Especialidad

| Médico              | Código  | Especialidad | Citas Prog. | Pacientes | Nivel de Carga |
|---------------------|---------|-------------|-------------|-----------|----------------|
| Dr. Roberto Juárez  | MED-001 | CARDIOLOGIA | 3           | 3         | BAJA           |
| Dra. Ana Solís      | MED-002 | NEUROLOGIA  | 1           | 1         | BAJA           |
| Dr. Luis Herrera    | MED-003 | PEDIATRIA   | 1           | 1         | BAJA           |
| Dra. Carmen Vega    | MED-004 | (error)     | 1           | 1         | BAJA           |

### Reporte 3 — Agenda de Citas

| Fecha      | Hora  | Paciente         | Médico              | Especialidad | Estado         |
|------------|-------|------------------|---------------------|-------------|----------------|
| 2025-04-10 | 09:00 | María García     | Dr. Roberto Juárez  | CARDIOLOGIA | CONFIRMADA     |
| 2025-04-10 | 09:00 | Pedro Castillo   | Dr. Roberto Juárez  | CARDIOLOGIA | ⚠ CONFLICTO   |
| 2025-04-10 | 11:30 | Carlos Mendoza   | Dra. Ana Solís      | NEUROLOGIA  | CONFIRMADA     |
| 2025-04-12 | 10:00 | Roberto Flores   | Dr. Roberto Juárez  | CARDIOLOGIA | CONFIRMADA     |

Nota: Las citas con fecha/hora inválida (2025-13-05 y 25:00) generan error léxico. Dependiendo de la implementación, pueden o no aparecer en la agenda.

**Conflictos detectados: 1** (Dr. Roberto Juárez, 2025-04-10, 09:00)

### Reporte 4 — Estadístico General

| Indicador                              | Valor                                              |
|----------------------------------------|----------------------------------------------------|
| Total de pacientes registrados         | 6                                                  |
| Total de médicos activos               | 4                                                  |
| Total de citas programadas             | 6 (4 válidas, 2 con error en fecha/hora)           |
| Citas con conflicto de horario         | 1 ⚠                                               |
| Pacientes con diagnóstico activo       | 4 de 6 (66.7%)                                     |
| Medicamento más prescrito              | Losartán 50mg / Furosemida 40mg (1 c/u)            |
| Especialidad con mayor carga de citas  | CARDIOLOGIA — Dr. Roberto Juárez (3 citas)         |
| Promedio de edad de los pacientes      | 50.0 años                                          |

---

## ARCHIVO 3: test_avanzado.med

### Errores Léxicos Esperados

| No. | Lexema          | Tipo de Error              | Descripción                                                                                       | Línea aprox. | Gravedad |
|-----|-----------------|---------------------------|----------------------------------------------------------------------------------------------------|-------------|----------|
| 1   | "O++"           | Tipo de sangre inválido   | 'O++' no es un tipo de sangre reconocido. Válidos: O+, O-, A+, A-, B+, B-, AB+, AB-.              | 7           | ERROR    |
| 2   | "C+"            | Tipo de sangre inválido   | 'C+' no es un tipo de sangre reconocido. Válidos: O+, O-, A+, A-, B+, B-, AB+, AB-.               | 9           | ERROR    |
| 3   | "Andrea Pineda  | Cadena sin cerrar         | Se encontró el inicio de una cadena pero no se detectó el cierre antes del fin de línea.           | 11          | CRÍTICO  |
| 4   | "XY"            | Tipo de sangre inválido   | 'XY' no es un tipo de sangre reconocido. Válidos: O+, O-, A+, A-, B+, B-, AB+, AB-.               | 14          | ERROR    |
| 5   | TRAUMATOLOGIA   | Especialidad no reconocida| 'TRAUMATOLOGIA' no es una especialidad válida. Use: CARDIOLOGIA, NEUROLOGIA, PEDIATRIA, CIRUGIA, MEDICINA_GENERAL u ONCOLOGIA. | 21 | ERROR |
| 6   | 2025-13-01      | Fecha con mes inválido    | El mes '13' está fuera del rango permitido (01-12). Formato esperado: AAAA-MM-DD.                  | 32          | ERROR    |
| 7   | 25:00           | Hora fuera de rango       | La hora '25:00' está fuera del rango permitido (00:00 - 23:59). Formato esperado: HH:MM.           | 33          | ERROR    |
| 8   | 2025-00-10      | Fecha con mes inválido    | El mes '00' está fuera del rango permitido (01-12). Formato esperado: AAAA-MM-DD.                  | 36          | ERROR    |
| 9   | 10:60           | Hora fuera de rango       | Los minutos '60' están fuera del rango permitido (00-59). Formato esperado: HH:MM.                 | 37          | ERROR    |
| 10  | 2025-04-32      | Fecha con día inválido    | El día '32' está fuera del rango permitido (01-31). Formato esperado: AAAA-MM-DD.                  | 38          | ERROR    |
| 11  | "Ceftriaxona 1g | Cadena sin cerrar         | Se encontró el inicio de una cadena pero no se detectó el cierre antes del fin esperado.           | 54          | CRÍTICO  |
| 12  | QUINCENAL       | Frecuencia de dosis no reconocida | 'QUINCENAL' no es una frecuencia válida. Use: DIARIA, CADA_8_HORAS, CADA_12_HORAS o SEMANAL. | 55 | ERROR |

**Total de errores: 12** (10 ERROR, 2 CRÍTICO)

### Reporte 1 — Historial de Pacientes

| Paciente           | Edad | Sangre  | Diagnóstico activo         | Medicamento / Dosis                    | Estado    |
|--------------------|------|---------|----------------------------|----------------------------------------|-----------|
| María García       | 45   | A+      | Hipertensión Arterial      | Losartán 50mg / DIARIA                 | ACTIVO    |
| Carlos Mendoza     | 62   | O-      | Diabetes Tipo 2            | Metformina 850mg / CADA_12_HORAS       | ACTIVO    |
| Sofía Ramírez      | 29   | B+      | Sin diagnóstico registrado | —                                      | SIN DIAG. |
| Pedro Castillo     | 71   | AB-     | Insuficiencia Cardiaca     | Furosemida 40mg / DIARIA               | ACTIVO    |
| Elena Morales      | 38   | (error) | Fractura de Fémur          | Ibuprofeno 600mg / CADA_8_HORAS        | ACTIVO    |
| Roberto Flores     | 55   | A-      | Arritmia Ventricular       | Amiodarona 200mg / SEMANAL             | ACTIVO    |
| Lucía Hernández    | 42   | (error) | Apendicitis Aguda          | (error en cadena)                      | ACTIVO    |
| Fernando Rivas     | 33   | AB+     | Resfriado Común            | Paracetamol 500mg / CADA_8_HORAS       | ACTIVO    |
| Andrea Pineda      | (error cadena) | — | —                     | —                                      | (error)   |
| Miguel Torres      | 58   | O+      | Cáncer de Pulmón / Neumonía| Cisplatino 50mg / SEMANAL + Azitromicina 500mg / DIARIA | CRÍTICO |
| Gabriela Ruiz      | 19   | A+      | Taquicardia Sinusal        | Metoprolol 25mg / CADA_12_HORAS        | ACTIVO    |
| Raúl Domínguez     | 66   | (error) | Hipertensión Arterial      | Losartán 50mg / (error dosis)          | ACTIVO    |

### Reporte 2 — Carga de Médicos por Especialidad

| Médico               | Código  | Especialidad     | Citas Prog. | Pacientes | Nivel de Carga |
|----------------------|---------|-----------------|-------------|-----------|----------------|
| Dr. Roberto Juárez   | MED-001 | CARDIOLOGIA     | 4           | 4         | NORMAL         |
| Dra. Ana Solís       | MED-002 | NEUROLOGIA      | 1           | 1         | BAJA           |
| Dr. Luis Herrera     | MED-003 | PEDIATRIA       | 2           | 2         | BAJA           |
| Dra. Carmen Vega     | MED-004 | CIRUGIA         | 2           | 2         | BAJA           |
| Dr. Marcos León      | MED-005 | MEDICINA_GENERAL| 2           | 2         | BAJA           |
| Dra. Patricia Reyes  | MED-006 | ONCOLOGIA       | 2           | 2         | BAJA           |
| Dr. Enrique Paz      | MED-007 | (error)         | 1           | 1         | BAJA           |
| Dra. Isabel Cruz     | MED-008 | CARDIOLOGIA     | 2           | 2         | BAJA           |

Nota: Los conteos exactos dependen de si la implementación incluye citas con fecha/hora inválida.

### Reporte 3 — Agenda de Citas (citas válidas, ordenadas)

| Fecha      | Hora  | Paciente         | Médico              | Especialidad     | Estado         |
|------------|-------|------------------|---------------------|-----------------|----------------|
| 2025-04-10 | 09:00 | María García     | Dr. Roberto Juárez  | CARDIOLOGIA     | CONFIRMADA     |
| 2025-04-10 | 09:00 | Pedro Castillo   | Dr. Roberto Juárez  | CARDIOLOGIA     | ⚠ CONFLICTO   |
| 2025-04-10 | 09:00 | Miguel Torres    | Dr. Roberto Juárez  | CARDIOLOGIA     | ⚠ CONFLICTO   |
| 2025-04-10 | 11:30 | Carlos Mendoza   | Dra. Ana Solís      | NEUROLOGIA      | CONFIRMADA     |
| 2025-04-11 | 08:00 | Sofía Ramírez    | Dr. Luis Herrera    | PEDIATRIA       | CONFIRMADA     |
| 2025-04-11 | 14:00 | Elena Morales    | Dra. Carmen Vega    | CIRUGIA         | CONFIRMADA     |
| 2025-04-11 | 14:00 | Lucía Hernández  | Dra. Carmen Vega    | CIRUGIA         | ⚠ CONFLICTO   |
| 2025-04-12 | 10:00 | Roberto Flores   | Dr. Roberto Juárez  | CARDIOLOGIA     | CONFIRMADA     |
| 2025-04-12 | 16:00 | Fernando Rivas   | Dr. Marcos León     | MEDICINA_GENERAL| CONFIRMADA     |
| 2025-04-12 | 16:00 | Raúl Domínguez   | Dr. Marcos León     | MEDICINA_GENERAL| ⚠ CONFLICTO   |
| 2025-04-15 | (error)| Gabriela Ruiz   | Dra. Isabel Cruz    | CARDIOLOGIA     | (error hora)   |
| 2025-04-18 | 09:00 | Sofía Ramírez    | Dra. Patricia Reyes | ONCOLOGIA       | CONFIRMADA     |
| 2025-04-19 | 15:30 | Pedro Castillo   | Dra. Isabel Cruz    | CARDIOLOGIA     | CONFIRMADA     |
| 2025-04-20 | (error)| Fernando Rivas  | Dr. Enrique Paz     | (error)         | (error hora)   |

**Conflictos detectados: 4**
1. Dr. Roberto Juárez — 2025-04-10, 09:00 (3 citas simultáneas)
2. Dra. Carmen Vega — 2025-04-11, 14:00 (2 citas simultáneas)
3. Dr. Marcos León — 2025-04-12, 16:00 (2 citas simultáneas)

### Reporte 4 — Estadístico General

| Indicador                              | Valor                                              |
|----------------------------------------|----------------------------------------------------|
| Total de pacientes registrados         | 12                                                 |
| Total de médicos activos               | 8                                                  |
| Total de citas programadas             | 17 (varias con errores de fecha/hora)              |
| Citas con conflicto de horario         | 4 ⚠                                               |
| Pacientes con diagnóstico activo       | 9 de 12 (75%)                                      |
| Medicamento más prescrito              | Losartán 50mg (2 pacientes)                        |
| Especialidad con mayor carga de citas  | CARDIOLOGIA (6 citas entre Juárez y Cruz)          |
| Promedio de edad de los pacientes      | 45.5 años                                          |

### Resumen de tipos de error cubiertos en este archivo

1. **Tipo de sangre inválido** — "O++", "C+", "XY" (3 casos)
2. **Cadena sin cerrar** — "Andrea Pineda (sin cierre), "Ceftriaxona 1g (sin cierre) (2 casos CRÍTICOS)
3. **Fecha con mes inválido** — 2025-13-01, 2025-00-10 (2 casos)
4. **Fecha con día inválido** — 2025-04-32 (1 caso)
5. **Hora fuera de rango** — 25:00, 10:60 (2 casos)
6. **Especialidad no reconocida** — TRAUMATOLOGIA (1 caso)
7. **Frecuencia de dosis no reconocida** — QUINCENAL (1 caso)
8. **Conflictos de horario** — 3 colisiones distintas (4 citas en conflicto)
9. **Paciente sin diagnóstico** — Sofía Ramírez
10. **Paciente con múltiples diagnósticos** — Miguel Torres (CRÍTICO)
