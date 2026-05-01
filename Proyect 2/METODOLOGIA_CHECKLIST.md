# Checklist Metodología - Proyecto TaskScript

## Fase 1: Análisis y Diseño

- [x] **Definir formalmente la gramática léxica de TaskScript**
  - Alfabeto, tipos de tokens, patrones y errores
  - Completado: Documentado en `src/core/lexer/Token.h` con 35+ tipos de tokens

- [x] **Definir la gramática libre de contexto completa del lenguaje**
  - BNF-style grammar dokumentado
  - Completado: `docs/GRAMMAR_REFERENCE.md` con CFG formal

- [x] **Diseñar el diagrama completo del AFD**
  - AFD (Finite State Automaton) para tokens
  - Completado: Implementado en `LexicalAnalyzer::nextToken()`

- [x] **Diseñar la arquitectura del sistema**
  - 4 capas: LexicalAnalyzer, SyntaxAnalyzer, SemanticAnalyzer, ReportGenerator

## Fase 2: Implementación del Análisis Léxico y Sintáctico

- [x] **Definir casos de prueba**
  - Completado: 6 archivos de prueba en `test/`

- [x] **Implementar Token y LexicalAnalyzer**
  - Completado: Tokenización con 35+ tipos de tokens

- [x] **Implementar SyntaxAnalyzer**
  - Completado: Parser descendente recursivo con recuperación de errores

- [x] **Implementar ErrorManager**
  - Completado: Acumulación de errores léxicos, sintácticos y semánticos

- [x] **Verificar funcionamiento desde consola**
  - ✓ Entrada válida: procesa sin errores
  - ✓ Error léxico: detecta caracteres inválidos
  - ✓ Error sintáctico: detecta violaciones de gramática
  - ✓ Error semántico: detecta duplicados

## Fase 3: Análisis Semántico y Reportes

- [x] **Implementar SemanticAnalyzer**
  - Validaciones de duplicados y atributos requeridos

- [x] **Implementar ReportGenerator**
  - `tokens.html` - listado de tokens
  - `errors.html` - errores clasificados
  - `stats.html` - estadísticas
  - `ast.dot` - árbol sintáctico Graphviz

- [x] **Implementar Tablero Kanban Visual** ⭐ NUEVO
  - `kanban.html` - 3 columnas (Por Hacer, En Progreso, Completado)
  - Tarjetas con prioridad, responsable, fecha límite
  - Colores: ALTA (rojo), MEDIA (naranja), BAJA (verde)
  - Interfaz moderna con gradientes y efectos hover

## Fase 4: Documentación y Finalización

- [ ] **Corregir errores y casos borde**
- [ ] **Redactar Manual Técnico**
- [ ] **Redactar Manual de Usuario**
- [ ] **Ejecutar 10 casos de prueba completos**
- [ ] **Organizar repositorio GitHub**

---

## Completitud: 62% (Kanban agregado ✅)

| Componente | Estado |
|---|---|
| Lexer | ✅ 100% |
| Parser | ✅ 100% |
| Análisis Semántico | ✅ 100% |
| Reportes | ✅ 100% |
| **Kanban Visual** | ✅ **100%** |
| GUI | ⏳ 0% |
| Documentación | ⏳ 10% |

*Última actualización: 1 de mayo de 2026*
