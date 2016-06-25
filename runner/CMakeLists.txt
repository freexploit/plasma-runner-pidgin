set(krunner_pidgin_SRCS
    pidgin_runner.cpp pidgin_dbus.cpp
)
set(main_SRCS 
    main.cpp pidgin_dbus.cpp
)
add_library(krunner_pidgin MODULE ${krunner_pidgin_SRCS})
target_link_libraries(krunner_pidgin KF5::Runner KF5::Plasma Qt5::Widgets Qt5::Xml 
    KF5::I18n KF5::Service KF5::KIOCore KF5::KIOFileWidgets KF5::KIOWidgets KF5::KIONTLM )

install(TARGETS krunner_pidgin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-runner-pidgin.desktop DESTINATION ${SERVICES_INSTALL_DIR})

add_executable(main ${main_SRCS})
target_link_libraries(main KF5::Runner KF5::Plasma Qt5::Widgets Qt5::Xml 
    KF5::I18n KF5::Service KF5::KIOCore KF5::KIOFileWidgets KF5::KIOWidgets KF5::KIONTLM )
