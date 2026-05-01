# DEPURACIÓN EN VS CODE - Proyecto 2 / TaskScript

## Configuración completada

Se han creado 3 archivos en `.vscode/`:
- **launch.json** - Configuraciones de depuración con GDB
- **tasks.json** - Tareas de compilación (configure, build, clean)

## Opción 1: Depuración RÁPIDA (Recomendado)

### Pasos:

1. **Abre VS Code** en la carpeta `Proyect 2`

2. **Compila en Debug** (primer intento):
   ```powershell
   # En la terminal PowerShell
   .\COMPILAR_DEBUG.ps1
   ```
   O ejecuta esta tarea en VS Code:
   - Presiona `Ctrl+Shift+B` → selecciona "cmake: configure (debug)"
   - Luego `Ctrl+Shift+B` → selecciona "cmake: build (debug)"

3. **Inicia depurador**:
   - Abre un archivo `.cpp` del proyecto (ej: `src/app/main.cpp`)
   - Presiona `F5` para iniciar depuración
   - Se abrirá la GUI de Qt

4. **Prueba la depuración**:
   - Pon un breakpoint haciendo clic en el número de línea (lado izquierdo)
   - Ejecuta código que alcance ese breakpoint
   - VS Code se detendrá y podrás inspeccionar variables

## Opción 2: Depuración MANUAL (Si F5 no funciona)

1. En VS Code, abre la paleta de comandos:
   - `Ctrl+Shift+P`

2. Escribe: `Debug: Select and Start Debugging`

3. Selecciona:
   - `GDB: GUI Debug (Qt Test)` para depurar la GUI
   - `GDB: CLI Debug` para depurar el CLI

4. El depurador se iniciará automáticamente

## Opción 3: Línea de Comandos PowerShell

Si necesitas compilar y depurar manualmente:

```powershell
# Compilar en DEBUG
.\COMPILAR_DEBUG.ps1

# Iniciar GUI
.\build\taskscript_qt_test.exe

# Depurar CLI con archivo de entrada
gdb.exe -ex "run test/test06_formato_usuario.task" .\build\taskscript_cli.exe
```

## Solución de Problemas

### "Program not found"
- Ejecuta: `.\COMPILAR_DEBUG.ps1`
- Verifica que `build/taskscript_qt_test.exe` existe

### GDB no inicia
- Verifica ruta: `C:/Qt/Tools/mingw1120_64/bin/gdb.exe`
- Si existe pero no funciona: actualiza PATH en `launch.json`

### GUI no se abre
- Verifica Qt: `qmake --version`
- Debe mostrar: "Qt version 6.5.3"

### Depurador se detiene inmediatamente
- Esto es normal si no hay breakpoints
- Añade un breakpoint en `src/app/main.cpp` línea 5
- Ejecuta de nuevo con `F5`

## Variables de Entorno Configuradas

El depurador incluye automáticamente:
- `C:/Qt/6.5.3/mingw_64/bin` - Bibliotecas Qt
- `C:/Qt/Tools/mingw1120_64/bin` - Compilador y depurador

## Atajos Útiles de Depuración

| Atajo | Acción |
|-------|--------|
| `F5` | Iniciar / Continuar depuración |
| `F10` | Siguiente línea (Step Over) |
| `F11` | Entrar en función (Step Into) |
| `Shift+F11` | Salir de función (Step Out) |
| `Ctrl+K Ctrl+I` | Ver variable bajo cursor |
| `Ctrl+Shift+D` | Abrir vista Debug |

## Próximos Pasos

Una vez que la depuración funcione:

1. **Añade más breakpoints** en funciones importantes:
   - `src/core/parser/SyntaxAnalyzer.cpp` - parser
   - `src/core/semantic/SemanticAnalyzer.cpp` - validación semántica
   - `src/core/report/Report.cpp` - generación de reportes

2. **Usa Watch Expressions** (Vista Debug):
   - Click derecho en variable → "Add to Watch"
   - Inspecciona estado en tiempo real

3. **Nota**: Esto es para GUI de prueba (`taskscript_qt_test.exe`)
   - Para implementar GUI completa, necesitarías editar `src/gui/`
