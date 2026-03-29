# Checklist Proyecto MedLang

## 1) Estado General

- [x] Lexer nativo implementado (sin std::regex, con nextToken)
- [x] Lectura de archivo .med desde consola
- [x] Reconocimiento de delimitadores base: { } [ ] : ; , -
- [x] Reconocimiento de literales basicos: ENTERO, CADENA, IDENTIFICADOR
- [ ] Parser sintactico del lenguaje completo
- [ ] Importacion a estructuras de datos (Paciente, Medico, Cita, Diagnostico)
- [ ] Validaciones semanticas (conflictos, referencias inexistentes)
- [ ] Reportes de errores lexico/sintactico/semantico

## 2) Requisitos Lexicos Solicitados (Checklist detallado)

### 2.1 Palabras reservadas
- [x] HOSPITAL
- [x] PACIENTES
- [x] MEDICOS
- [x] CITAS
- [x] DIAGNOSTICOS
- [x] Singulares en minuscula: paciente, medico, cita, diagnostico

### 2.2 Codigos con guion (ej. PAC-001, MED-023)
- [ ] Token unico CODIGO_ID
- [x] Soporte parcial actual (se tokeniza como IDENTIFICADOR, Minus, ENTERO)

### 2.3 Literales de fecha (AAAA-MM-DD)
- [ ] Token unico FECHA_LITERAL
- [x] Soporte parcial actual (se tokeniza como ENTERO, Minus, ENTERO, Minus, ENTERO)

### 2.4 Literales de hora (HH:MM)
- [ ] Token unico HORA_LITERAL
- [x] Soporte parcial actual (se tokeniza como ENTERO, Colon, ENTERO)

### 2.5 Enumeraciones y restricciones
- [ ] Especialidades validas (ej. CARDIOLOGIA, NEUROLOGIA, etc.)
- [ ] Dosis valida (ej. DIARIA, CADA_8_HORAS, etc.)
- [ ] Tipo de sangre restringido (A+, A-, B+, B-, AB+, AB-, O+, O-)

### 2.6 Delimitadores estructurales
- [x] { }
- [x] [ ]
- [x] : ; , -

## 3) Tabla de Errores Lexicos (Requerimiento del proyecto)

Debe existir una tabla acumulada de errores sin detener el analisis.

Columnas minimas:
1. Numero
2. Lexema invalido
3. Tipo de error
4. Descripcion
5. Posicion exacta (linea, columna)

Estado actual:
- [ ] Tabla de errores implementada
- [x] El analisis puede continuar tras token Unknown (no aborta todo el proceso)

## 4) Reglas Semanticas a agregar (post-parser)

- [ ] Detectar conflicto de horario entre citas (mismo medico y misma fecha/hora)
- [ ] Detectar cita con paciente inexistente
- [ ] Detectar cita con medico inexistente
- [ ] Detectar codigos de medico duplicados

## 5) Plan de Trabajo Recomendado

1. Completar lexer avanzado:
   - CODIGO_ID
   - FECHA_LITERAL
   - HORA_LITERAL
   - tabla formal de errores lexicos

2. Implementar parser:
   - parseHospital
   - parsePacientes
   - parseMedicos
   - parseCitas
   - parseDiagnosticos

3. Construir modelo de datos:
   - struct Paciente
   - struct Medico
   - struct Cita
   - struct Diagnostico

4. Validaciones semanticas:
   - conflictos de horario
   - referencias cruzadas
   - restricciones de catalogos

5. Reportes finales:
   - tokens
   - errores lexicos
   - errores sintacticos
   - errores semanticos
