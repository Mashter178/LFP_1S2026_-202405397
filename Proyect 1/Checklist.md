# Checklist General Proyecto MedLang

## 1. Hitos de Entrega
- [ ] Código completo en C++ organizado en clases y módulos, debidamente comentado
- [x] Proyecto compila con g++ -std=c++17 o QMake/CMake
- [x] Clase principal de la GUI
- [x] GUI en Qt 5.15/6.x o wxWidgets 3.x
- [x] Proyecto con CMakeLists.txt o QMake (.pro)
- [ ] README con compilación paso a paso

## 2. Núcleo del Lenguaje
- [x] Lenguaje base en C++17 o superior
- [x] Prohibido std::regex en el lexer
- [x] AFD implementado con transiciones explícitas en nextToken()
- [x] Definir estructura global y tokens del lenguaje
- [x] Clase Token (lexema, tipo, línea, columna)
- [x] Clase LexicalAnalyzer con método nextToken() basado en el AFD (implementada como `Lexer::nextToken()`)
- [ ] Clase ErrorManager
- [ ] Afinar casos ambiguos y falsos positivos
- [ ] Documentar formalmente el AFD (diagrama y tabla)
- [ ] Validar el AFD con casos de prueba de borde

## 3. Parser y Semántica
- [x] Implementar parser sintáctico por bloques
- [x] Construir estructuras de datos: Paciente, Medico, Cita, Diagnóstico
- [x] Validar referencias cruzadas y conflictos de horario
- [ ] Detectar códigos duplicados
- [x] Reporte de errores sintácticos y semánticos en consola
- [x] Exportar reporte semántico HTML

## 4. Reportes HTML
- [x] Exportar reporte léxico HTML
- [x] Clase ReportGenerator
- [x] Reporte 1: Historial de Pacientes
- [x] Reporte 2: Carga de Médicos por Especialidad
- [x] Reporte 3: Agenda de Citas con conflictos
- [x] Reporte 4: Estadístico General del Hospital
- [x] Índice HTML generado y apertura directa desde GUI
- [x] Exportación robusta con errores: índice parcial + enlace a reporte léxico
- [x] Exportar Graphviz (.dot) con jerarquía y relaciones (`output/hospital.dot`)

## 5. GUI
- [x] Integrar carga de archivo .med desde interfaz
- [x] Mostrar tabla de tokens y errores en GUI
- [x] Botones para análisis y exportar reportes
- [x] Componentes mínimos en GUI: área de texto, tabla de tokens, tabla de errores, botones de carga/análisis, acceso a reportes HTML

## 6. Pruebas y Cobertura
- [x] Pruebas manuales base (caso válido y con conflictos)
- [x] Prueba de exportación HTML en escenarios con errores léxicos/sintácticos
- [ ] Batería de pruebas válidas por bloque
- [ ] Batería de pruebas inválidas (errores de símbolos, fecha, hora, código)
- [ ] Verificar cobertura de todos los tipos de token
- [ ] Corregir errores detectados por pruebas

## 7. Documentación y Cierre
- [ ] Manual técnico: arquitectura del sistema (diagrama de componentes UML)
- [ ] Manual técnico: diagrama de clases completo
- [ ] Manual técnico: diagrama del AFD y tabla de transiciones
- [ ] Manual técnico: descripción de algoritmos y justificación de diseño
- [ ] Manual de usuario: capturas de pantalla de la GUI
- [ ] Manual de usuario: guía de compilación e instalación de dependencias
- [ ] Manual de usuario: ejemplos de archivo .med y explicación de estructura
- [ ] Manual de usuario: instrucciones para analizar archivo y leer reportes
- [ ] Documentar arquitectura y funcionamiento de nextToken
- [ ] Documentar formato de entrada .med con ejemplos
- [ ] Documentar tabla de errores y criterios de recuperación

## 8. Pendientes Priorizados de Backend
- [ ] Implementar detección semántica de códigos duplicados en médicos
- [ ] Completar pruebas formales: suite de casos válidos e inválidos por bloque
- [x] Exportar reporte Graphviz (.dot) con jerarquía y relaciones

## 9. Estado Final Consolidado
- [x] Base funcional completa del proyecto (lexer + parser + semántica base + GUI + reportes)
- [x] Flujo estable de compilación/ejecución con scripts PowerShell
- [x] Exportación de reportes en escenarios válidos y con errores
- [ ] Pendiente de cierre académico: documentación completa (manual técnico + manual de usuario)
- [ ] Pendiente de robustez: pruebas formales completas y cobertura
- [ ] Pendiente funcional de backend: detección de códigos duplicados en médicos
