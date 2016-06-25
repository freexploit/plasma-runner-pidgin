cmake_minimum_required(VERSION 2.8.12)
project(PidginRunner)

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set (CMAKE_MODULE_PATH
     ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH}
     CMAKE_EXPORT_COMPILE_COMMANDS 1
)

# Find the required Libaries
find_package (Qt5 REQUIRED CONFIG COMPONENTS Widgets Core Xml)
find_package (KF5 REQUIRED COMPONENTS KIO I18n Service Runner Plasma)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

add_subdirectory(runner)


