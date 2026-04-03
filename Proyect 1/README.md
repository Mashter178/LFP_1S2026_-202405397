# MedLang - Compilador de Gestión Hospitalaria

Compilador especializado para análisis y generación de reportes de datos hospitalarios. Implementa análisis léxico, sintáctico y semántico para un lenguaje de dominio específico (DSL) de estructura hospitalaria.

**Versión**: 1.0  
**Lenguaje**: C++17  
**Framework GUI**: Qt 6.5.3  
**Compilador**: MinGW G++ 11.2.0

---

## 🚀 Quick Start (5 minutos)

### Compilación Rápida

```powershell
cd "C:\Users\<Usuario>\OneDrive\Documentos\VSC\LFP\Proyectos\Proyect 1"

# Configurar
C:\Qt\Tools\CMake_64\bin\cmake.exe -S . -B build -G "MinGW Makefiles" `
    -DCMAKE_CXX_COMPILER="C:/Qt/Tools/mingw1120_64/bin/g++.exe" `
    -DCMAKE_PREFIX_PATH="C:/Qt/6.5.3/mingw_64"

# Compilar
C:\Qt\Tools\CMake_64\bin\cmake.exe --build build -j
```

**Esperado:**
```
[100%] Built target medlang_cli
[100%] Built target medlang_gui
```

### Ejecutar GUI

```powershell
.\build\medlang_gui.exe
```

### Ejecutar CLI

```powershell
.\build\medlang_cli.exe test/hospital_valido_01.med
```

Reportes generados en: `output/indice_reportes.html` ✅

---

## 📋 Características

### Análisis Completo (3 fases)

| Fase | Tarea | Salida |
|------|-------|--------|
| **Léxica** | Tokenización y validación de símbolos | reporte_lexico.html |
| **Sintáctica** | Parsing descendente recursivo | Validación de estructura |
| **Semántica** | Validación de reglas y referencias | Detección de conflictos |

### 6 Reportes HTML + Diagrama DOT

1. **Reporte Léxico** - Tabla de tokens con errores
2. **Historial de Pacientes** - Status clínico (color-coded)
3. **Carga de Médicos** - Workload por especialista
4. **Agenda de Citas** - Cronograma con detección de conflictos
5. **Estadístico General** - Métricas agregadas del hospital
6. **Diagrama Graphviz** - Visualización de estructura

---

## 📁 Estructura de Proyecto

```
Proyect 1/
├── src/               ← Código fuente C++
├── build/             ← Binarios compilados
├── test/              ← Archivos .med de ejemplo
├── output/            ← Reportes generados (HTML + DOT)
├── docs/              ← Documentación
│   ├── manual_tecnico.md     (Arquitectura, AFD, implementación)
│   ├── manual_usuario.md     (Instalación, uso, ejemplos)
│   └── README.md             (Este archivo)
├── CMakeLists.txt     ← Configuración de build
└── Checklist.md       ← Estado del proyecto
```

---

## 📖 Documentación

Para documentación detallada:

**👨‍💻 Desarrolladores:**
→ Lee [`docs/manual_tecnico.md`](docs/manual_tecnico.md)
- Arquitectura de capas
- AFD y gramática del lenguaje
- Implementación de Lexer/Parser/Analyzer
- Decisiones de diseño

**👤 Usuarios / Administradores:**
→ Lee [`docs/manual_usuario.md`](docs/manual_usuario.md)
- Instalación step-by-step
- Interfaz GUI y CLI
- Formato del archivo `.med`
- Interpretación de reportes
- Troubleshooting

---

## 🔧 Requisitos del Sistema

| Componente | Versión |
|-----------|---------|
| Windows | 10/11 |
| CMake | 3.21+ |
| MinGW G++ | 11.2.0 |
| Qt Framework | 6.5.3 |
| Graphviz | 10.0+ (opcional) |

### Verificar Requisitos

```powershell
cmake --version          # Esperado: 3.21+
g++ --version            # Esperado: MinGW 11.2.0
qmake --version          # Esperado: Qt 6.5.3
```

---

## ✨ Ejemplo de Uso

### Crear archivo `hospital.med`

```
HOSPITAL {
    PACIENTES {
        paciente: "María López" [edad: 34, tipo_sangre: "A+", habitacion: 101],
        paciente: "Carlos Mendoza" [edad: 62, tipo_sangre: "O-", habitacion: 204],
    };
    
    MEDICOS {
        medico: "Dr. Roberto Juárez" [especialidad: CARDIOLOGIA, codigo: "MED-001"],
        medico: "Dra. Ana Solís" [especialidad: NEUROLOGIA, codigo: "MED-002"],
    };
    
    CITAS {
        cita: "María López" con "Dr. Robert Juárez" [fecha: 2025-04-10, hora: 08:00],
        cita: "Carlos Mendoza" con "Dra. Ana Solís" [fecha: 2025-04-10, hora: 09:30],
    };
    
    DIAGNOSTICOS {
        diagnostico: "María López" [condicion: "Hipertensión", medicamento: "Losartán 50mg", dosis: DIARIA],
        diagnostico: "Carlos Mendoza" [condicion: "Diabetes grave", medicamento: "Insulina", dosis: CADA_12_HORAS],
    };
};
```

### Analizar desde Terminal

```powershell
.\build\medlang_cli.exe hospital.med
```

### Resultado

```
✅ Reporte léxico: output/reporte_lexico.html
✅ Historial pacientes: output/historial_pacientes.html
✅ Carga médicos: output/carga_medicos_especialidad.html
✅ Agenda citas: output/agenda_citas_conflictos.html
✅ Estadístico: output/estadistico_general_hospital.html
✅ Diagrama: output/hospital.dot
✅ Índice: output/indice_reportes.html
```

Abre `output/indice_reportes.html` en tu navegador → ¡Listo! 🎉

---

## 📊 Validaciones Semánticas

MedLang detecta automáticamente:

- ✅ Referencias inválidas (paciente/médico no existe)
- ✅ Conflictos de horario (mismo médico, misma fecha/hora)
- ✅ Duplicados de códigos médicos
- ✅ Diagnósticos críticos (medicación urgente)
- ✅ Carga de médicos (BAJA, NORMAL, ALTA, SATURADA)

---

## 🎨 Interpretación de Reportes

**Colores:**
- 🟢 Verde = Normal / OK
- 🟠 Naranja = Advertencia / Pendiente
- 🔴 Rojo = Crítico / Error / Conflicto
- 🔵 Azul = Información / BAJA carga

---

## 🛠️ Compilación Desde VS Code

Presionar `Ctrl+Shift+B` y seleccionar:
1. `MedLang: Configure CMake` (primera vez)
2. `MedLang: Build` (compilación)
3. `MedLang: Run GUI` (ejecutar)

---

## 📝 Archivos de Ejemplo

En carpeta `test/`:
- `hospital_valido_01.med` ✅ Válido - Genera todos los reportes
- `hospital_valido_02.med` ✅ Válido - Casos de diagnósticos críticos
- `hospital_errores_01.med` ❌ Con conflictos - Genera reporte léxico
- `hospital_errores_02.med` ❌ Referencias inválidas - Fallback

---

## 🐛 Troubleshooting Rápido

| Problema | Solución |
|----------|----------|
| `cmake: command not found` | Agregar `C:\Qt\Tools\CMake_64\bin` a PATH |
| `Cannot find Qt` | Actualizar CMAKE_PREFIX_PATH en CMakeLists.txt |
| GUI no abre | Closerar proceso anterior y recompilar |
| Reportes no generan | Verificar archivo `.med` es válido (ver manual) |

---

## 📚 Documentos

- **docs/manual_tecnico.md** - Arquitectura, AFD, implementación
- **docs/manual_usuario.md** - Instalación, uso, ejemplos
- **Checklist.md** - Estado del proyecto
- **test/*.med** - Archivos de ejemplo

---

## 🔗 Recursos

- [Qt 6 Documentation](https://doc.qt.io/qt-6/)
- [CMake Documentation](https://cmake.org/documentation/)
- [Graphviz](https://graphviz.org/)
- [MinGW-w64](https://www.mingw-w64.org/)

---

## ✅ Testing

```powershell
# Test válido
.\build\medlang_cli.exe test/hospital_valido_01.med
# Esperado: 7 archivos en output/

# Test con errores
.\build\medlang_cli.exe test/hospital_errores_01.med
# Esperado: reporte_lexico.html + fallback index
```

---

## 📋 Checklist de Características

- ✅ Análisis léxico con AFD
- ✅ Parsing sintáctico descendente recursivo
- ✅ Validación semántica de referencias
- ✅ Detección de conflictos horarios
- ✅ 5 reportes HTML con color-coding
- ✅ Diagrama Graphviz DOT
- ✅ GUI con Qt 6
- ✅ CLI standalone
- ✅ Fallback index para errores
- ✅ Documentación técnica y de usuario

---

## 📄 Licencia

Proyecto académico de Lenguajes Formales y de Programación (LFP).

---

## 👤 Autor

**MedLang Development Team**  
Versión 1.0 - Abril 2026

---

**¿Necesitas ayuda?**
- Instalación: Ver `docs/manual_usuario.md` § 2
- Uso: Ver `docs/manual_usuario.md` § 4-6
- Arquitectura: Ver `docs/manual_tecnico.md` § 2-5
- Troubleshooting: Ver `docs/manual_usuario.md` § 7
