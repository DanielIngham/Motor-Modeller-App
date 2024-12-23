cmake_minimum_required(VERSION 3.16)

project(MotorModeller VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Gui Core Widgets Sql)
find_package(Qt6 6.4 REQUIRED COMPONENTS SerialPort)

qt_standard_project_setup()

qt_add_executable(appMotorModeller
    main.cpp
)

qt_add_qml_module(appMotorModeller
    URI MotorModeller
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES serialreciever.h serialreciever.cpp
    SOURCES datahandler.h datahandler.cpp
    QML_FILES Offset.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appMotorModeller PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appMotorModeller
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appMotorModeller
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Gui
    PRIVATE Qt6::Core
    PRIVATE Qt6::Widgets
    PRIVATE Qt6::Sql
)

target_link_libraries(appMotorModeller PRIVATE Qt6::SerialPort)

include(GNUInstallDirs)
install(TARGETS appMotorModeller
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
