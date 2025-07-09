cmake_minimum_required(VERSION 3.16)

project(ECG_GUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -funroll-loops")

# Tambahkan QtSerialPort
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport SerialPort)

# Jika butuh translasi
set(TS_FILES ECG_GUI_en_ID.ts)

# QCustomPlot path
set(Dependencies
    depedencies/qcustomplot.cpp
    depedencies/qcustomplot.h
)

# Project source files
set(PROJECT_SOURCES
    src/src/main.cpp
    src/src/mainwindow.cpp
    src/include/mainwindow.h
    src/src/mainwindow.ui
    ${Dependencies}
    # ${TS_FILES}
)

# Build target
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ECG_GUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    # qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    add_executable(ECG_GUI ${PROJECT_SOURCES})
    # qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# Include path
target_include_directories(ECG_GUI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/depedencies
)

# Link Qt libraries (tambahkan SerialPort)
target_link_libraries(ECG_GUI PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::SerialPort
)

# Optional: macOS/iOS bundle
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ECG_GUI)
endif()

set_target_properties(ECG_GUI PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install
include(GNUInstallDirs)
install(TARGETS ECG_GUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize (Qt6)
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ECG_GUI)
endif()
