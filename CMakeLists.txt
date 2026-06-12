cmake_minimum_required(VERSION 3.16)

project(MonitoringProdEquipment VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ищем Qt (только один раз!)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Определяем версию
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    set(QT_MAJOR_VERSION 6)
else()
    set(QT_MAJOR_VERSION 5)
endif()

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    machine.cpp
    machine.h
    emulatormanager.cpp
    emulatormanager.h
    logs.cpp
    logs.h
    logs.ui
    checkmachinestatus.cpp
    checkmachinestatus.h
    checkmachinestatus.ui
    addmachineinlist.cpp
    addmachineinlist.h
    addmachineinlist.ui
)

# Создание таргета
if(QT_MAJOR_VERSION EQUAL 6)
    qt_add_executable(MonitoringProdEquipment
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(MonitoringProdEquipment SHARED ${PROJECT_SOURCES})
    else()
        add_executable(MonitoringProdEquipment ${PROJECT_SOURCES})
    endif()
endif()

# Линковка ПОСЛЕ создания таргета
target_link_libraries(MonitoringProdEquipment PRIVATE
    Qt${QT_MAJOR_VERSION}::Widgets
    Qt${QT_MAJOR_VERSION}::Sql
)

# Настройки для iOS/macOS
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MonitoringProdEquipment)
endif()

set_target_properties(MonitoringProdEquipment PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MonitoringProdEquipment
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_MAJOR_VERSION EQUAL 6)
    qt_finalize_executable(MonitoringProdEquipment)
endif()