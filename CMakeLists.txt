cmake_minimum_required(VERSION 3.16)

project(MonitoringProdEquipment VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(APP_SOURCES
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    machine.cpp machine.h
    emulatormanager.cpp emulatormanager.h
    logs.cpp logs.h logs.ui
    checkmachinestatus.cpp checkmachinestatus.h checkmachinestatus.ui
    addmachineinlist.cpp addmachineinlist.h addmachineinlist.ui
    resources.qrc
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_executable(MonitoringProdEquipment
        MANUAL_FINALIZATION
        ${APP_SOURCES}
    )
else()
    add_executable(MonitoringProdEquipment ${APP_SOURCES})
endif()

target_link_libraries(MonitoringProdEquipment PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

set_target_properties(MonitoringProdEquipment PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER "dev.lamer002.monitoring"
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MonitoringProdEquipment
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(CTest)
if(BUILD_TESTING)
    add_executable(monitoring_core_tests
        tests/machine_tests.cpp
        machine.cpp
    )
    target_include_directories(monitoring_core_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    add_test(NAME monitoring_core_tests COMMAND monitoring_core_tests)
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MonitoringProdEquipment)
endif()
