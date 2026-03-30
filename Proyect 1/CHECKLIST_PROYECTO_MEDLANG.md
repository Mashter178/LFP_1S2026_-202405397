# Checklist General Proyecto MedLang

## Fase 1. Analisis del Lenguaje y Diseno del AFD

- [x] Definir estructura global del lenguaje: HOSPITAL, PACIENTES, MEDICOS, CITAS, DIAGNOSTICOS
- [x] Definir convencion de palabras reservadas (plurales en MAYUSCULA, singulares en minuscula)
- [x] Definir delimitadores estructurales: { } [ ] : ; , -
- [x] Definir literales base: ENTERO, CADENA, IDENTIFICADOR
- [x] Definir tokens compuestos requeridos: FECHA_LITERAL, HORA_LITERAL, CODIGO_ID
- [x] Definir catalogos: especialidades, frecuencia de dosis, tipos de sangre
- [ ] Documentar formalmente el AFD por estados y transiciones (diagrama)
- [ ] Validar el AFD con casos de prueba de borde

## Fase 2. Implementacion del Motor Lexico

- [x] Implementar lexer nativo en C++ (sin std::regex)
- [x] Implementar lectura de archivo .med
- [x] Implementar nextToken con avance continuo del flujo
- [x] Reconocer palabras reservadas del lenguaje
- [x] Reconocer FECHA_LITERAL (AAAA-MM-DD)
- [x] Reconocer HORA_LITERAL (HH:MM)
- [x] Reconocer CODIGO_ID (AAA-999...)
- [x] Reconocer enumeraciones: ESPECIALIDAD_ENUM y DOSIS_ENUM
- [x] Reconocer tipo de sangre restringido como TIPO_SANGRE_LITERAL
- [x] Implementar tabla de errores lexicos acumulada
- [x] Continuar el analisis tras error lexico sin detener ejecucion
- [ ] Afinar clasificacion de casos ambiguos (ejemplo: codigo invalido dentro de CADENA)

## Fase 3. Integracion con GUI y Generacion de Reportes

- [ ] Definir tecnologia GUI (Qt solo si el curso lo exige)
- [ ] Integrar carga de archivo .med desde interfaz
- [ ] Mostrar tabla de tokens en GUI
- [ ] Mostrar tabla de errores lexicos en GUI
- [x] Exportar reporte lexico HTML (tabla de tokens y tabla de errores)
- [ ] Exportar Reporte 1 HTML: Historial de Pacientes
- [ ] Columnas Reporte 1: Nombre, Edad, Sangre, Diagnostico activo, Medicamento, Estado (colores)
- [ ] Exportar Reporte 2 HTML: Carga de Medicos por Especialidad
- [ ] Columnas Reporte 2: Nombre, Codigo, Especialidad, Citas programadas, Numero de pacientes, Nivel de carga (colores)
- [ ] Exportar Reporte 3 HTML: Agenda de Citas con deteccion de conflictos
- [ ] Columnas Reporte 3: Fecha, Hora, Paciente, Medico, Especialidad, Estado (colores)
- [ ] Exportar Reporte 4 HTML: Estadistico General del Hospital
- [ ] Tabla Reporte 4-A: resumen general hospitalario
- [ ] Tabla Reporte 4-B: desglose especifico por especialidad
- [ ] Exportar Graphviz (.dot) con jerarquia y relaciones del hospital analizado

## Fase 4. Pruebas, Correccion y Documentacion

- [ ] Crear bateria de pruebas validas por bloque (PACIENTES, MEDICOS, CITAS, DIAGNOSTICOS)
- [ ] Crear bateria de pruebas invalidas (errores de simbolos, fecha, hora, codigo)
- [ ] Verificar cobertura de todos los tipos de token
- [ ] Corregir errores detectados por pruebas
- [ ] Documentar arquitectura del lexer y funcionamiento de nextToken
- [ ] Documentar formato de entrada .med con ejemplos
- [ ] Documentar tabla de errores y criterios de recuperacion

## Fase 5. Siguiente Entregable Tecnico (Parser + Semantica)

- [ ] Implementar parser sintactico por bloques
- [ ] Construir estructuras de datos: Paciente, Medico, Cita, Diagnostico
- [ ] Validar referencias cruzadas (paciente/medico existentes)
- [ ] Detectar conflicto de horario entre citas (mismo medico, misma fecha/hora)
- [ ] Detectar codigos duplicados
- [ ] Generar reporte de errores sintacticos y semanticos
