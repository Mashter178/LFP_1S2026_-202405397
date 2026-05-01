cmake_minimum_required(VERSION 3.21)

project(TaskScript LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable Qt automatic moc for any Qt targets
set(CMAKE_AUTOMOC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_library(taskscript_core STATIC
    src/core/lexer/LexicalAnalyzer.cpp
    src/core/parser/SyntaxAnalyzer.cpp
    src/core/semantic/SemanticAnalyzer.cpp
    src/core/report/ReportGenerator.cpp
    src/core/report/KanbanReportGenerator.cpp
    src/core/report/WorkloadReportGenerator.cpp
)

target_include_directories(taskscript_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(taskscript_cli
    src/app/main.cpp
)

target_link_libraries(taskscript_cli PRIVATE taskscript_core)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(taskscript_qt_test
    src/gui/qt_test_main.cpp
)

target_link_libraries(taskscript_qt_test PRIVATE Qt6::Widgets)
