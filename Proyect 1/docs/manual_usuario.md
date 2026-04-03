# Manual de Usuario - MedLang

## 1. Requisitos del Sistema

### Hardware Mínimo
- **Procesador**: Intel i5 o equivalente (2 GHz)
- **RAM**: 4 GB
- **Espacio en disco**: 500 MB (código fuente + binarios)

### Software Requerido

| Componente | Versión | Descripción |
|-----------|---------|------------|
| **CMake** | 3.21+ | Sistema de build |
| **MinGW** | 11.2.0 | Compilador G++ C++17 |
| **Qt Framework** | 6.5.3 | Librerías GUI |
| **Graphviz** | 10.0+ | Generación de diagramas (opcional) |
| **Windows** | 10/11 | Sistema operativo |

### Verificación de Requisitos

```powershell
# Verificar CMake
cmake --version
# Esperado: cmake version 3.21 o superior

# Verificar compilador
g++ --version
# Esperado: g++ 11.2.0 (MinGW)

# Verificar Qt
qmake --version
# Esperado: Qt version 6.5.3
```

---

## 2. Instalación y Compilación

### 2.1 Instalación de Dependencias

#### Opción A: Instalación Manual (Recomendado)

**1. Qt Framework 6.5.3**
- Descargar de: https://www.qt.io/download-open-source
- Instalador recomendado: Qt Creator con componentes MinGW 11.2.0
- Ruta sugerida: `C:\Qt\6.5.3\mingw_64`

**2. CMake 3.21+**
- Descargar de: https://cmake.org/download/
- Instalador: cmake-X.XX.X-windows-x86_64.msi
- Agregar a PATH durante instalación

**3. MinGW G++ 11.2.0**
- Incluido con Qt Creator o descargar de: https://www.mingw-w64.org/
- Ruta sugerida: `C:\Qt\Tools\mingw1120_64\bin`

#### Opción B: Usando Qt Creator (Todo integrado)
- Instalar Qt Creator IDE
- Qt Creator automáticamente instala CMake, MinGW y Qt

### 2.2 Compilación desde Terminal

#### Paso 1: Navegar al directorio del proyecto
```powershell
cd "C:\Users\<Usuario>\OneDrive\Documentos\VSC\LFP\Proyectos\Proyect 1"
```

#### Paso 2: Configurar CMake
```powershell
C:\Qt\Tools\CMake_64\bin\cmake.exe `
    -S . `
    -B build `
    -G "MinGW Makefiles" `
    -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1120_64/bin/g++.exe" `
    -DCMAKE_PREFIX_PATH="C:/Qt/6.5.3/mingw_64"
```

**Salida esperada:**
```
-- The CXX compiler identification is GNU ...
-- Check for working CXX compiler: ...
-- Configuring done (XX.X s)
-- Generating done
-- Build files have been written to: .../build
```

#### Paso 3: Compilar
```powershell
C:\Qt\Tools\CMake_64\bin\cmake.exe --build build -j
```

**Salida esperada:**
```
[ 5%] Building CXX object CMakeFiles/medlang_core.dir/...
...
[100%] Built target medlang_cli
[100%] Built target medlang_gui
```

**Salida final:**
```
EXIT: 0
```

### 2.3 Compilación desde VS Code

**Usando las tareas predefinidas:**

1. Presionar `Ctrl+Shift+B` (Run Build Task)
2. Seleccionar: `MedLang: Configure CMake`
3. Esperar a que termine
4. Presionar `Ctrl+Shift+B` nuevamente
5. Seleccionar: `MedLang: Build`

**Alternativa rápida:**
- Presionar `Ctrl+Shift+P`
- Buscar: "Tasks: Run Task"
- Seleccionar: "MedLang: Build"

### 2.4 Troubleshooting de Compilación

| Error | Causa | Solución |
|-------|-------|----------|
| `Cannot find Qt` | Qt no en PATH | Actualizar CMAKE_PREFIX_PATH en CMakeLists.txt |
| `g++ not found` | MinGW no instalado | Instalar desde https://www.mingw-w64.org/ |
| `cmake: command not found` | CMake no en PATH | Agregar `C:\Qt\Tools\CMake_64\bin` a PATH |
| `Permission denied` | Archivo .exe locked | Usar carpeta build_alt (VS Code lo hace automáticamente) |

---

## 3. Interfaz de Línea de Comandos (CLI)

### 3.1 Uso Básico

```powershell
cd "Proyect 1"
.\build\medlang_cli.exe test/hospital_valido_01.med
```

### 3.2 Parámetros

```
Uso: medlang_cli.exe <archivo.med> [opciones]

Parámetros:
  <archivo.med>      Archivo de entrada (requerido)
  
Salida:
  - Reportes en carpeta: output/
  - Índice: output/indice_reportes.html
```

### 3.3 Ejemplo de Ejecución Completa

```powershell
# 1. Compilar
C:\Qt\Tools\CMake_64\bin\cmake.exe --build build -j

# 2. Ejecutar análisis
.\build\medlang_cli.exe test/hospital_valido_01.med

# 3. Ver resultados
explorer.exe output\indice_reportes.html
```

**Salida esperada en consola:**
```
Analysis started...
Lexical phase: 50 tokens parsed
Syntax phase: Hospital structure validated
Semantic phase: No conflicts detected
Reports generated successfully
Output folder: output/
```

---

## 4. Interfaz Gráfica (GUI)

### 4.1 Inicio de la Aplicación

```powershell
cd "Proyect 1"
.\build\medlang_gui.exe
```

### 4.2 Componentes Principales

#### Barra Superior
- **Abrir archivo**: Botón para seleccionar archivo .med
- **Analizar**: Ejecuta léxico + sintáctico + semántico
- **Exportar**: Genera reportes HTML y DOT

#### Panel Izquierdo (Preview)
- Muestra contenido del archivo .med cargado
- Editable para pruebas rápidas

#### Panel Central (Tokens)
- Tabla con todos los tokens reconocidos
- Columnas: Número, Lexema, Tipo, Línea, Columna

#### Panel Inferior (Errores)
- **Tab Léxicos**: Errores de tokenización
- **Tab Sintácticos**: Errores de estructura
- **Tab Semánticos**: Errores de lógica

### 4.3 Flujo de Trabajo Típico

```
┌─────────────────────────────────┐
│ 1. Click: "Abrir archivo"       │
│    Seleccionar: hospital.med    │
└──┬──────────────────────────────┘
   │
   ↓
┌─────────────────────────────────┐
│ 2. El archivo se carga en      │
│    preview (panel izquierdo)    │
└──┬──────────────────────────────┘
   │
   ↓
┌─────────────────────────────────┐
│ 3. Click: "Analizar"            │
│    - Lexical scan               │
│    - Parse syntax               │
│    - Semantic check             │
└──┬──────────────────────────────┘
   │
   ↓
┌─────────────────────────────────┐
│ 4. Resultados:                  │
│    - Tokens panel (actualizados)│
│    - Error tabs (si existen)    │
└──┬──────────────────────────────┘
   │
   ↓
┌─────────────────────────────────┐
│ 5. Click: "Exportar"            │
│    Genera 6 archivos HTML + DOT │
│    Abre índice en navegador     │
└─────────────────────────────────┘
```

### 4.4 Interpretación de Colores

| Color | Significado |
|-------|------------|
| 🟢 Verde | Token válido / Status OK |
| 🟠 Naranja | Advertencia / Pendiente |
| 🔴 Rojo | Error / Crítico |
| 🔵 Azul | Información / BAJA carga |

---

## 5. Formato del Archivo .med

### 5.1 Estructura General

```
HOSPITAL {
    PACIENTES { ... };
    MEDICOS { ... };
    CITAS { ... };
    DIAGNOSTICOS { ... };
};
```

### 5.2 Bloque PACIENTES

```
PACIENTES {
    paciente: "Nombre Completo" [edad: 34, tipo_sangre: "A+", habitacion: 101],
    paciente: "Otro Paciente" [edad: 45, tipo_sangre: "O-", habitacion: 205],
};
```

**Atributos obligatorios:**
- `edad`: Número entero (ej: 34)
- `tipo_sangre`: A+, A-, B+, B-, O+, O-, AB+, AB-
- `habitacion`: Número de habitación

**Validaciones:**
- Nombre: Mínimo 3 caracteres, contiene espacios
- Edad: Número válido
- Tipo sangre: Uno de los 8 valores permitidos

### 5.3 Bloque MEDICOS

```
MEDICOS {
    medico: "Dr. Roberto Juárez" [especialidad: CARDIOLOGIA, codigo: "MED-001"],
    medico: "Dra. Ana Solís" [especialidad: NEUROLOGIA, codigo: "MED-002"],
};
```

**Atributos obligatorios:**
- `especialidad`: Categoría médica (ver tabla abajo)
- `codigo`: Formato MED-XXX donde XXX son 3 dígitos

**Especialidades válidas:**
```
CARDIOLOGIA, NEUROLOGIA, PEDIATRIA, CIRUGIA, 
MEDICINA_GENERAL, ONCOLOGIA, OTORRINO, OFTALMOLOGIA,
DERMATOLOGIA, ENDOCRINOLOGIA, GASTROENTEROLOGIA,
UROLOGIA, PSIQUIATRIA, ORTOPEDIA, NEUMOLOGIA
```

**Validaciones:**
- Código: Debe ser MED-### (exactamente)
- No pueden existir dos médicos con el mismo código
- Especialidad: Debe estar en lista permitida

### 5.4 Bloque CITAS

```
CITAS {
    cita: "Nombre Paciente" con "Dr. Nombre Médico" [fecha: 2025-04-10, hora: 08:00],
    cita: "Otro Paciente" con "Dra. Otra Médica" [fecha: 2025-04-11, hora: 14:30],
};
```

**Atributos obligatorios:**
- `fecha`: Formato YYYY-MM-DD
- `hora`: Formato HH:MM (00:00-23:59)

**Validaciones:**
- Paciente: Debe existir en bloque PACIENTES
- Médico: Debe existir en bloque MEDICOS
- Fecha: Formato correcto YYYY-MM-DD
- Hora: Formato correcto HH:MM
- Conflicto: Mismo médico no puede tener 2 citas en misma fecha/hora

### 5.5 Bloque DIAGNOSTICOS

```
DIAGNOSTICOS {
    diagnostico: "Nombre Paciente" [condicion: "Hipertensión Arterial", medicamento: "Losartán 50mg", dosis: DIARIA],
    diagnostico: "Otro Paciente" [condicion: "Diabetes Tipo 2", medicamento: "Insulina", dosis: CADA_12_HORAS],
};
```

**Atributos obligatorios:**
- `condicion`: Descripción de la enfermedad
- `medicamento`: Nombre + dosis del fármaco
- `dosis`: Frecuencia de administración

**Frecuencias válidas:**
```
CADA_HORA, CADA_2_HORAS, CADA_4_HORAS, CADA_6_HORAS,
CADA_8_HORAS, CADA_12_HORAS, CADA_24_HORAS, DIARIA,
SEMANAL, MENSUAL, SEGUN_SEA_NECESARIO
```

**Clasificación de Diagnósticos:**
- **CRÍTICO** (Color Rojo): 
  - Contiene: "crítico", "grave", "severo", "urgente"
  - O dosis frecuencia ≤ 4 horas
- **NORMAL**: Resto de casos
- **ACTIVO**: Registrado en el sistema

**Validaciones:**
- Paciente: Debe existir en bloque PACIENTES
- Dosis: Debe estar en lista permitida

### 5.6 Ejemplo Completo: hospital_valido_01.med

```
HOSPITAL {

    PACIENTES {
        paciente: "María Fernanda López" [edad: 34, tipo_sangre: "A+", habitacion: 101],
        paciente: "Carlos Enrique Mendoza" [edad: 62, tipo_sangre: "O-", habitacion: 204],
        paciente: "Sofía Alejandra Ramírez" [edad: 29, tipo_sangre: "B+", habitacion: 310],
        paciente: "Pedro Antonio Castillo" [edad: 71, tipo_sangre: "AB-", habitacion: 415],
        paciente: "Lucía Beatriz Herrera" [edad: 45, tipo_sangre: "O+", habitacion: 502],
    };

    MEDICOS {
        medico: "Dr. Roberto Juárez" [especialidad: CARDIOLOGIA, codigo: "MED-001"],
        medico: "Dra. Ana Solís" [especialidad: NEUROLOGIA, codigo: "MED-002"],
        medico: "Dr. Luis Herrera" [especialidad: PEDIATRIA, codigo: "MED-003"],
        medico: "Dra. Carmen Vega" [especialidad: CIRUGIA, codigo: "MED-004"],
        medico: "Dr. Miguel Ángel Torres" [especialidad: MEDICINA_GENERAL, codigo: "MED-005"],
    };

    CITAS {
        cita: "María Fernanda López" con "Dr. Roberto Juárez" [fecha: 2025-04-10, hora: 08:00],
        cita: "Carlos Enrique Mendoza" con "Dra. Ana Solís" [fecha: 2025-04-10, hora: 09:30],
        cita: "Pedro Antonio Castillo" con "Dr. Roberto Juárez" [fecha: 2025-04-11, hora: 08:30],
        cita: "Lucía Beatriz Herrera" con "Dra. Carmen Vega" [fecha: 2025-04-11, hora: 11:00],
    };

    DIAGNOSTICOS {
        diagnostico: "María Fernanda López" [condicion: "Hipertensión Arterial", medicamento: "Losartán 50mg", dosis: DIARIA],
        diagnostico: "Carlos Enrique Mendoza" [condicion: "Diabetes Tipo 2", medicamento: "Metformina 850mg", dosis: CADA_12_HORAS],
        diagnostico: "Pedro Antonio Castillo" [condicion: "Insuficiencia Cardiaca grave", medicamento: "Furosemida 40mg", dosis: DIARIA],
        diagnostico: "Lucía Beatriz Herrera" [condicion: "Cáncer de Mama Etapa II", medicamento: "Tamoxifeno 20mg", dosis: DIARIA],
    };

};
```

---

## 6. Interpretación de Reportes

### 6.1 Índice Principal (indice_reportes.html)

**Punto de entrada** para todos los análisis. Contiene links a:
1. **Reporte Léxico** - Todos los tokens y errores encontrados
2. **Historial de Pacientes** - Status de cada paciente
3. **Carga de Médicos** - Workload por especialista
4. **Agenda de Citas** - Calendario de atenciones
5. **Estadístico General** - Métricas agregadas del hospital
6. **Diagrama Graphviz** - Estructura visual del hospital

### 6.2 Reporte Léxico (reporte_lexico.html)

**Contenido:** Tabla de todos los tokens reconocidos

| Número | Lexema | Tipo | Línea | Columna | Descripción |
|--------|--------|------|-------|---------|------------|
| 1 | HOSPITAL | Keyword | 1 | 1 | Palabra clave del programa |
| 2 | { | Operator | 1 | 10 | Apertura de bloque |
| ... | ... | ... | ... | ... | ... |

**Errores mostrados:**
- Identificadores inválidos
- Formatos de fecha incorrecto
- Typos sugeridos

**Acción:** Revisar errores léxicos si el parser falla

### 6.3 Historial de Pacientes (historial_pacientes.html)

**Contenido:** Tabla con estado clínico de cada paciente

| Paciente | Edad | Tipo Sangre | Diagnóstico Activo | Medicamento/Dosis | Estado |
|----------|------|-------------|-------------------|------------------|--------|
| María López | 34 | A+ | Hipertensión | Losartán 50mg / DIARIA | 🟢 Normal |
| Pedro Castillo | 71 | AB- | Insuficiencia Cardiaca grave | Furosemida 40mg / DIARIA | 🔴 Crítico |

**Leyenda de Estados:**
- 🟢 **Normal**: Diagnóstico estable, medicación regular
- 🟡 **Advertencia**: Medicación frecuente (≤4 horas)
- 🔴 **Crítico**: Condición grave o medicación de urgencia

**Cómo leer:**
1. Filtrar porcolor para identificar pacientes críticos rápidamente
2. Revisar medicamento para entender tratamiento
3. Contactar especialista si hay cambios en estado

### 6.4 Carga de Médicos (carga_medicos_especialidad.html)

**Contenido:** Workload de cada médico

| Médico | Código | Especialidad | Citas Prog. | Pacientes Únicos | Nivel de Carga |
|--------|--------|--------------|------------|-----------------|-----------------|
| Dr. Juárez | MED-001 | CARDIOLOGIA | 3 | 3 | 🟢 BAJA |
| Dra. Alonzo | MED-006 | ONCOLOGIA | 9 | 7 | 🔴 SATURADA |

**Niveles de Carga:**
- 🟢 **BAJA** (≤2 citas): Disponibilidad alta
- 🟢 **NORMAL** (3-5 citas): Carga balanceada
- 🟠 **ALTA** (6-8 citas): Carga elevada, posibles retrasos
- 🔴 **SATURADA** (9+): Sobrecarga, se requiere redistribución

**Cómo usar:**
1. Nuevo turno: Asignar a médico con carga BAJA o NORMAL
2. Planificación: Identificar especialidades con sobrecarga
3. Recursos: Considerar contratar si muchos médicos en SATURADA

### 6.5 Agenda de Citas (agenda_citas_conflictos.html)

**Contenido:** Cronograma de atenciones con detección de conflictos

| Fecha | Hora | Paciente | Médico | Especialidad | Estado |
|-------|------|----------|--------|--------------|--------|
| 2025-04-10 | 08:00 | María López | Dr. Juárez | CARDIOLOGIA | 🟢 OK |
| 2025-04-10 | 08:00 | Carlos Mendoza | Dr. Juárez | CARDIOLOGIA | 🔴 CONFLICTO |

**Estados de Cita:**
- 🟢 **Confirmada**: Dentro de horario, sin conflictos
- 🟠 **Pendiente**: Futura, requiere confirmación del paciente
- 🔴 **Conflicto**: Mismo médico/hora/fecha - **Requiere resolución inmediata**

**Cómo resolver conflictos:**
1. Identificar citas con 🔴 CONFLICTO
2. Opciones:
   - Reprogramar una cita a otra hora
   - Reasignar a otro médico de misma especialidad
   - Consultar disponibilidad de ambas partes

### 6.6 Estadístico General (estadistico_general_hospital.html)

**Contenido:** Métricas agregadas del hospital

| Métrica | Valor |
|---------|-------|
| Total de Pacientes | 12 |
| Promedio de Edad | 45.3 años |
| Pacientes Críticos | 2 |
| Total de Médicos | 7 |
| Specialties Registradas | 8 |
| Citas Programadas | 12 |
| Medicamento más usado | Insulina (3 pacientes) |
| Tipo de Sangre Prevalente | O+ |

**Interpretación:**
- **Pacientes Críticos > 20%**: Hospital requiere refuerzo en áreas críticas
- **Promedio de edad > 65**: Recursos geriátricos importantes
- **Citas ≤ doctores**: Carga balanceada
- **Medicamento top**: Indicador de enfermedades prevalentes

### 6.7 Diagrama Graphviz (hospital.dot)

**Contenido:** Estructura visual del hospital en formato DOT

**Estructura:**
```
HOSPITAL (raíz)
  ├─ PACIENTES (carpeta)
  │   ├─ p1: María López (34 años)
  │   ├─ p2: Carlos Mendoza (62 años)
  │   └─ ...
  ├─ MEDICOS (carpeta)
  │   ├─ m1: Dr. Juárez (MED-001)
  │   ├─ m2: Dra. Solís (MED-002)
  │   └─ ...
  ├─ CITAS (carpeta)
  │   ├─ [Edges: p1 → m1, etiqueta: 2025-04-10 08:00]
  │   └─ ...
  └─ DIAGNOSTICOS (carpeta)
      ├─ d1: María López - Hipertensión
      └─ ...
```

**Cómo visualizar:**
1. Opción A: Usar Graphviz Online (https://dreampuf.github.io/GraphvizOnline/)
   - Copiar contenido de hospital.dot
   - Visualizar interactivamente
2. Opción B: Instalar Graphviz localmente
   ```powershell
   dot -Tpng hospital.dot -o hospital.png
   ```
3. Opción C: PlantUML (https://www.plantuml.com/plantuml/uml/)

**Colores:**
- 🟢 Verde: Entidades normales
- 🔴 Rojo: Conflictos detectados (edge en rojo)
- 🔵 Azul: Información adicional

**Casos de uso:**
- Visualizar complejidad del hospital
- Detectar patrones de referencia médica
- Validar integridad de relaciones
- Documentación de estructura

---

## 7. Guía de Solución de Problemas

### 7.1 El Programa No Abre

**Síntoma:** No se ejecuta medlang_gui.exe

**Soluciones:**
1. Verificar que la compilación fue exitosa (EXIT: 0)
2. Verificar Qt 6.5.3 esté instalado correctamente
3. Probar desde línea de comandos:
   ```powershell
   .\build\medlang_gui.exe
   ```

### 7.2 Errores de Análisis No Esperados

**Síntoma:** Archivo válido reporta errores

**Verificar:**
1. Formato exacto (mayúsculas, espacios)
2. Que todos los bloque terminen con `;`
3. Que nombre de paciente/médico esté en comillas
4. Que las fechas usen formato YYYY-MM-DD

**Referencia:**
- Ver sección 5: Formato del archivo .med

### 7.3 Reportes No Se Generan

**Síntoma:** Click en "Exportar" no produce archivos

**Verificar:**
1. El análisis fue exitoso (sin errores sintácticos)
2. La carpeta `output/` existe (se crea automáticamente)
3. Permisos de escritura en carpeta output
4. Espacio en disco disponible

**Solución de permisos:**
```powershell
# Resetear permisos
icacls "output" /grant Everyone:F /t
```

### 7.4 Compilación Falla

**Error típico:** `Cannot find Qt`

**Solución:**
1. Verificar PATH:
   ```powershell
   $env:PATH -split ';' | grep -i qt
   ```
2. Actualizar CMAKE_PREFIX_PATH en CMakeLists.txt

**Error típico:** `g++ not found`

**Solución:**
1. Verificar MinGW instalado:
   ```powershell
   C:\Qt\Tools\mingw1120_64\bin\g++.exe --version
   ```
2. Si no existe, descargar desde https://www.mingw-w64.org/

### 7.5 Archivos Bloqueados

**Síntoma:** "Permission denied" al compilar

**Causa:** GuiI.exe está ejecutándose

**Soluciones:**
1. Cerrar ventana de GUI
2. En VS Code: Build automáticamente usa carpeta build_alt
3. Terminalmente:
   ```powershell
   Stop-Process -Name "medlang_gui" -Force
   # Luego recompilar
   ```

### 7.6 Port 8000 ya está en uso

**Síntoma:** GUI no abre archivos HTML

**Solución:**
1. Cerrar otros servidores locales
2. Usar navegador directo:
   ```powershell
   explorer.exe (Get-Item .\output\indice_reportes.html).FullName
   ```

---

## 8. Casos de Uso Comunes

### Caso 1: Analizar Hospital Pequeño (< 50 pacientes)

```powershell
# 1. Crear archivo hospital.med con estructura válida
# 2. Guardar en test/
# 3. Desde GUI:
#    - Abrir archivo
#    - Click "Analizar"
#    - Click "Exportar"
#    - Navegar índice

.\build\medlang_gui.exe
```

### Caso 2: Detectar Conflictos de Citas

```powershell
# Ver archivo: test/hospital_errores_01.med
# Contiene conflictos premeditados

.\build\medlang_cli.exe test/hospital_errores_01.med

# Revisar: output/agenda_citas_conflictos.html
# Buscar citas con color 🔴 CONFLICTO
```

### Caso 3: Identificar Médicos Sobrecargados

```powershell
# Analizar hospital válido

# Revisar: output/carga_medicos_especialidad.html
# Filtra por nivel 🔴 SATURADA
# --> Estos necesitan ayuda
```

### Caso 4: Documentar Estado del Hospital

```powershell
# Generar reporte completo

.\build\medlang_cli.exe test/hospital_valido_01.med

# Compartir carpeta output/ como:
# - Email a administración
# - SharePoint/OneDrive
# - Documentación médica
```

---

## 9. Keyboard Shortcuts (GUI)

| Atajo | Acción |
|-------|--------|
| `Ctrl+O` | Abrir archivo |
| `Ctrl+A` | Analizar |
| `Ctrl+E` | Exportar reportes |
| `Ctrl+Q` | Salir aplicación |
| `F5` | Refrescar |

---

## 10. Contacto y Soporte

**Preguntas técnicas:**
- Revisar Manual Técnico (docs/manual_tecnico.md)
- Verificar ejemplos en test/

**Reportar bugs:**
- Adjuntar archivo .med problemático
- Descripción del error esperado
- Versión de Windows/Qt/CMake

---

## Apéndice A: Archivos de Ejemplo

### hospital_valido_01.med
- ✅ Sintaxis correcta
- ✅ Sin conflictos de citas
- ✅ Todos los términos válidos
- 📊 Producción: 7 reportes HTML + 1 DOT

### hospital_valido_02.med
- ✅ Casos de diagnósticos críticos
- ✅ Mix de especialidades
- 📊 Producción: completa

### hospital_errores_01.med
- ❌ Conflictos de citas
- ❌ Typos en palabras clave
- 📊 Producción: reporte_lexico.html solo (fallback)

### hospital_errores_02.med
- ❌ Referencias inválidas (paciente no existe)
- ❌ Códigos de médicos duplicados
- 📊 Producción: reporte_lexico.html solo (fallback)

---

## Apéndice B: Referencia Rápida de Comandos

```powershell
# Compilar
cmake --build "Proyect 1/build" -j

# Ejecutar CLI
"Proyect 1/build/medlang_cli.exe" "Proyect 1/test/hospital_valido_01.med"

# Ejecutar GUI
"Proyect 1/build/medlang_gui.exe"

# Ver reportes
explorer "Proyect 1/output"
```

---

**Versión**: 1.0  
**Última actualización**: Abril 2026  
**Para**: Usuarios, administradores hospitalarios, evaluadores académicos
