cmake_minimum_required(VERSION 3.16)

project(Bluetooth VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Bluetooth)

include_directories(${PROJECT_SOURCE_DIR})

qt_add_executable(appBluetooth
    src/main.cpp
)

if(QT_KNOWN_POLICY_QTP0001)
    qt6_policy(SET QTP0001 NEW)
endif()

qt6_add_qml_module(appBluetooth
    URI Bluetooth
    VERSION 1.0
    QML_FILES ui/Main.qml
    QML_FILES ui/screens/HomeScreen.qml
    SOURCES src/common.h
    SOURCES src/chatserver.h src/chatserver.cpp
    SOURCES src/bluetoothdevicesmodel.h src/bluetoothdevicesmodel.cpp
    SOURCES src/bluetoothmanager.h src/bluetoothmanager.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
#set_target_properties(appBluetooth PROPERTIES
##    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appBluetooth
#    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
#    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
#    MACOSX_BUNDLE TRUE
#    WIN32_EXECUTABLE TRUE
#)

target_link_libraries(appBluetooth
    PRIVATE Qt6::Quick Qt6::Bluetooth
)

include(GNUInstallDirs)
install(TARGETS appBluetooth
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
