cmake_minimum_required(VERSION 3.21)
project(MedLang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MEDLANG_BUILD_GUI "Build Qt GUI executable" ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(MEDLANG_CORE_SOURCES
    src/app/MedLangService.cpp
    src/core/lexer/Lexer.cpp
    src/core/lexer/LexerCatalog.cpp
    src/core/lexer/LexerValidators.cpp
    src/core/parser/Parser.cpp
    src/core/report/Appointments.cpp
    src/core/report/Doctors.cpp
    src/core/report/Generator.cpp
    src/core/report/Hospital.cpp
    src/core/report/Html.cpp
    src/core/report/LexicalReport.cpp
    src/core/report/Patients.cpp
    src/core/semantic/SemanticAnalyzer.cpp
)

add_library(medlang_core STATIC ${MEDLANG_CORE_SOURCES})
target_include_directories(medlang_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(medlang_cli src/app/main.cpp)
target_link_libraries(medlang_cli PRIVATE medlang_core)

if(MEDLANG_BUILD_GUI)
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

    add_executable(medlang_gui
        src/gui/main.cpp
        src/gui/MainWindow.cpp
        src/gui/MainWindow.h
    )

    target_link_libraries(medlang_gui PRIVATE medlang_core Qt${QT_VERSION_MAJOR}::Widgets)
endif()
