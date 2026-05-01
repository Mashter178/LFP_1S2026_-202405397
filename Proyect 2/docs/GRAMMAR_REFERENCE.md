/* GRAMÁTICA LIBRE DE CONTEXTO (CFG) - TaskScript Language
 * 
 * Una Gramática Libre de Contexto (CFG) es un conjunto formal de reglas que definen
 * qué combinaciones de tokens forman oraciones válidas en el lenguaje.
 * 
 * Estructura básica de una regla:
 *   Símbolo No-Terminal → secuencia de símbolos (terminales y no-terminales)
 * 
 * SÍMBOLOS:
 *   - Terminales: palabras/tokens reales (entre " ")
 *   - No-Terminales: conceptos abstractos que se expanden en otras reglas
 *   - * : cero o más repeticiones
 *   - + : una o más repeticiones
 *   - ? : cero o una ocurrencia
 *   - | : alternativa (O)
 * 
 * ============================================================================
 * GRAMÁTICA ACTUAL DE TaskScript (Implementada en SyntaxAnalyzer.cpp)
 * ============================================================================
 */

/* NIVEL TOP (Programa completo) */
Programa → Tablero*

/* NIVEL 1: Tablero (contenedor principal) */
Tablero → "TABLERO" NombreTablero "{" Columna* "}" ";"?

NombreTablero → CADENA | IDENTIFICADOR

/* NIVEL 2: Columnas (estados del flujo) */
Columna → "COLUMNA" NombreColumna "{" Tarea* "}" ";"?

NombreColumna → CADENA | IDENTIFICADOR

/* NIVEL 3: Tareas (elementos de trabajo) */
Tarea → "tarea" ":" NombreTarea "[" Atributo* "]" ","?

NombreTarea → CADENA | IDENTIFICADOR

Atributo → AtributosPrioridad
         | AtributosResponsable
         | AtributosFechaLimite

AtributosPrioridad → "prioridad" ":" NivelPrioridad ","?

NivelPrioridad → "ALTA" | "MEDIA" | "BAJA"

AtributosResponsable → "responsable" ":" CADENA ","?

AtributosFechaLimite → "fecha_limite" ":" FECHA_LITERAL ","?

/* TERMINALES (tokens reales del lexer) */
CADENA → "..." (texto entre comillas)
IDENTIFICADOR → nombre_variable (sin comillas, válido en lenguaje)
FECHA_LITERAL → YYYY-MM-DD (validado por lexer)

============================================================================
EJEMPLO DE ENTRADA CONFORME A ESTA GRAMÁTICA:
============================================================================

TABLERO "Mi Proyecto" {
    COLUMNA "Por Hacer" {
        tarea: "Tarea 1" [prioridad: ALTA, responsable: "Juan", fecha_limite: 2026-05-15],
        tarea: "Tarea 2" [prioridad: MEDIA, responsable: "María", fecha_limite: 2026-05-20],
    };
    COLUMNA "Hecho" {
        tarea: "Tarea 3" [prioridad: BAJA, responsable: "Pedro", fecha_limite: 2026-04-30],
    };
};

============================================================================
CÓMO EXTENDER LA GRAMÁTICA (Ejemplos)
============================================================================

1. AGREGAR NUEVO ATRIBUTO OBLIGATORIO (por ejemplo: Descripción de tarea)
   
   Actual:
   Tarea → "tarea" ":" NombreTarea "[" Atributo* "]"
   
   Nuevo:
   Tarea → "tarea" ":" NombreTarea "[" Atributo* "]" descripcion ":" CADENA
   
   Resultado: Ahora toda tarea puede tener descripción.
   Entrada válida sería:
     tarea: "Mi Tarea" [prioridad: ALTA] descripcion: "Hacer el análisis"

2. AGREGAR ESTADO OPCIONAL DENTRO DE ATRIBUTOS

   Actual:
   Atributo → AtributosPrioridad | AtributosResponsable | AtributosFechaLimite
   
   Nuevo:
   Atributo → AtributosPrioridad 
            | AtributosResponsable 
            | AtributosFechaLimite
            | AtributosEstado

   AtributosEstado → "estado" ":" EstadoTarea ","?
   
   EstadoTarea → "PENDIENTE" | "EN_PROGRESO" | "BLOQUEADA" | "COMPLETADA"

3. PERMITIR ETIQUETAS EN TAREAS

   Actual:
   Tarea → "tarea" ":" NombreTarea "[" Atributo* "]" ","?
   
   Nuevo:
   Tarea → "tarea" ":" NombreTarea "{" Etiqueta* "}" "[" Atributo* "]" ","?
   
   Etiqueta → "#" IDENTIFICADOR

   Entrada válida:
     tarea: "Revisar código" #urgente #backend [prioridad: ALTA]

4. AGREGAR SUBTAREAS (recursión)

   Actual:
   Tarea → "tarea" ":" NombreTarea "[" Atributo* "]" ","?
   
   Nuevo:
   Tarea → "tarea" ":" NombreTarea "[" Atributo* "]" Subtareas? ","?
   
   Subtareas → "{" Tarea* "}"

   Entrada válida:
     tarea: "Feature Grande" [prioridad: ALTA] {
         tarea: "Subtarea 1" [prioridad: ALTA],
         tarea: "Subtarea 2" [prioridad: MEDIA],
     }

============================================================================
CÓMO MODIFICAR EN EL CÓDIGO (SyntaxAnalyzer.cpp)
============================================================================

Paso 1: Definir Token nuevo en Token.h (si lo necesitas)
  enum class TokenType {
      ESTADO,
      PENDIENTE,
      ...
  };

Paso 2: Agregar keyword en LexicalAnalyzer.cpp::initializeKeywords()
  m_keywords["ESTADO"] = TokenType::ESTADO;
  m_keywords["PENDIENTE"] = TokenType::PENDIENTE;

Paso 3: Agregar método privado en SyntaxAnalyzer.h
  private:
    ASTNode* parseAtributoEstado();

Paso 4: Implementar en SyntaxAnalyzer.cpp
  ASTNode* SyntaxAnalyzer::parseAtributoEstado() {
      Token t = advance(); // consume "estado"
      consumeExpected(TokenType::Colon, "Se esperaba ':' después de estado");
      if (m_current.type == TokenType::PENDIENTE || ...) {
          ASTNode* n = new ASTNode{"Estado", m_current.lexeme, ...};
          advance();
          return n;
      }
      // error handling...
  }

Paso 5: Integrar en parseTask()
  // Dentro de parseTask(), dentro del while que lee atributos:
  if (m_current.type == TokenType::ESTADO) {
      ASTNode* est = parseAtributoEstado();
      if (est) node->children.push_back(est);
  }

Paso 6: Validar en SemanticAnalyzer.cpp::analyzeTask()
  // Verificar que estado es válido, no duplicado, etc.

============================================================================
REFERENCIAS ÚTILES
============================================================================

- Recursive Descent Parser (lo que usamos):
  Técnica donde cada no-terminal es una función.
  parseProgram() → parseBoard() → parseColumn() → parseTask()

- Error Recovery (sincronización):
  Cuando hay error, saltamos hasta un token conocido (TABLERO, COLUMNA, etc.)
  para continuar parseando el resto.

- AST (Abstract Syntax Tree):
  El árbol que construimos donde cada nodo es un ASTNode con label, value, children.

- Left Recursion:
  Si una regla se expande a sí misma al inicio, el parser recursive descent
  se queda en bucle infinito. TaskScript NO tiene left recursion.

============================================================================
