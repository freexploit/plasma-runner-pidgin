project(pidginplasmarunner)
cmake_minimum_required(VERSION 2.8)

set(KDE_MIN_VERSION "4.3.60") # for the < 4.2 macro
find_package(KDE4 4.3.60 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -std=c++11)
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

set(example_SRCS pidginrunner.cpp pidginrunner.h pidgindbusconnector.cpp pidgindbusconnector.h)
kde4_add_plugin(krunner_pidginrunner ${example_SRCS})
target_link_libraries(krunner_pidginrunner ${KDE4_PLASMA_LIBS}  ${KDE4_KIO_LIBS})

install(TARGETS krunner_pidginrunner DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-runner-pidgin.desktop DESTINATION ${SERVICES_INSTALL_DIR})

add_executable(tester ${example_SRCS} main.cpp)
target_link_libraries(tester ${KDE4_PLASMA_LIBS}  ${KDE4_KIO_LIBS} ${Boost_LIBRARIES})

#aux_source_directory(. SRC_LIST)
#add_executable(${PROJECT_NAME} ${SRC_LIST})

