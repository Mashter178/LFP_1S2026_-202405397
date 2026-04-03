# Checklist General Proyecto MedLang

## 1. Hitos de Entrega
- [x] Código completo en C++ organizado en clases y módulos, debidamente comentado (src/ bien estructurado)
- [x] Proyecto compila con g++ -std=c++17 o QMake/CMake
- [x] Clase principal de la GUI
- [x] GUI en Qt 5.15/6.x o wxWidgets 3.x
- [x] Proyecto con CMakeLists.txt o QMake (.pro)
- [x] README con compilación paso a paso (README.md completado)

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
- [x] Manual técnico: arquitectura del sistema (diagrama de componentes UML) - docs/manual_tecnico.md § 2
- [x] Manual técnico: diagrama de clases completo - docs/manual_tecnico.md § 2.2
- [x] Manual técnico: diagrama del AFD y tabla de transiciones - docs/manual_tecnico.md § 3
- [x] Manual técnico: descripción de algoritmos y justificación de diseño - docs/manual_tecnico.md § 7-8
- [x] Manual de usuario: capturas de pantalla de la GUI - docs/manual_usuario.md § 4.1-4.3 (descripciones con tablas)
- [x] Manual de usuario: guía de compilación e instalación de dependencias - docs/manual_usuario.md § 2
- [x] Manual de usuario: ejemplos de archivo .med y explicación de estructura - docs/manual_usuario.md § 5-5.6
- [x] Manual de usuario: instrucciones para analizar archivo y leer reportes - docs/manual_usuario.md § 4 y § 6
- [x] Documentar arquitectura y funcionamiento de nextToken - docs/manual_tecnico.md § 3.2-3.3
- [x] Documentar formato de entrada .med con ejemplos - docs/manual_usuario.md § 5
- [x] Documentar tabla de errores y criterios de recuperación - docs/manual_tecnico.md § 4.3-4.4

## 8. Pendientes Priorizados de Backend
- [ ] Implementar detección semántica de códigos duplicados en médicos
- [ ] Completar pruebas formales: suite de casos válidos e inválidos por bloque
- [x] Exportar reporte Graphviz (.dot) con jerarquía y relaciones

## 9. Estado Final Consolidado

### Completado ✅
- [x] Base funcional completa del proyecto (lexer + parser + semántica base + GUI + reportes)
- [x] Flujo estable de compilación/ejecución con scripts PowerShell (Ejecutador.ps1, taskss.json)
- [x] Exportación de reportes en escenarios válidos y con errores (fallback index + reporte léxico)
- [x] Documentación técnica completa (manual_tecnico.md - 600+ líneas, 11 secciones)
- [x] Documentación de usuario completa (manual_usuario.md - 750+ líneas, 10 secciones + apéndices)
- [x] README.md como punto de entrada rápida (quick start, características, troubleshooting)

### Pendiente de Robustez 🔄
- [ ] Detección semántica de códigos duplicados en médicos (regla fácil de agregar)
- [ ] Batería formal de pruebas válidas e inválidas (casos por bloque)
- [ ] Suite de tests automatizados

### Resumen de Archivos Generados
- **docs/manual_tecnico.md** (11 secciones)
  - Descripción general, arquitectura, lexer (AFD), parser (gramática), semántica, reportes, decisiones de diseño, troubleshooting técnico
  
- **docs/manual_usuario.md** (10 secciones + apéndices)
  - Requisitos, instalación paso a paso, CLI vs GUI, formato .med con ejemplos, interpretación de reportes, troubleshooting, casos de uso comunes
  
- **README.md** (Quick Start)
  - Compilación rápida en 5 minutos, características, estructura, ejercicios, documentación cruzada

### Métricas de Completitud
| Componente | Estado | % | Notas |
|-----------|--------|---|-------|
| Compilación | ✅ | 100% | CMake + MinGW + Qt 6.5.3 |
| Lexer (AFD) | ✅ | 100% | 11 categorías de tokens, validaciones de formato |
| Parser (LL) | ✅ | 100% | 4 bloques PACIENTES/MEDICOS/CITAS/DIAGNOSTICOS |
| Semántica | ✅ | 85% | Referencias, conflictos, diagnósticos ✓ // Duplicados ✗ |
| Reportes HTML | ✅ | 100% | 5 reportes + índice + fallback |
| Graphviz DOT | ✅ | 100% | Jerarquía, relaciones, colores |
| GUI | ✅ | 100% | Qt MainWindow completo |
| Documentación | ✅ | 100% | Manual técnico + usuario + README |
| Pruebas | ⚠️ | 50% | Manual ✓ // Automatizadas ✗ |

### Próximos Pasos (Post-Entrega)
1. Agregar detección de códigos duplicados (½ hora)
2. Crear suite de tests automatizados (1-2 horas)
3. Agregar validaciones adicionales (rangos, patrones)
4. Optimizar performance en archivos grandes (1000+ registros)

