cmake_minimum_required(VERSION 3.19)
project(miniSerialMonitor LANGUAGES CXX)

find_package(Qt6 6 REQUIRED COMPONENTS Core Widgets SerialPort)

qt_standard_project_setup()

qt_add_executable(miniSerialMonitor
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    serial.h serial.cpp
    icons.qrc
    csv_save.h csv_save.cpp
    breeze.qrc
    settings.h settings.cpp settings.ui
)

target_link_libraries(miniSerialMonitor
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::SerialPort
)

include(GNUInstallDirs)

install(TARGETS miniSerialMonitor
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
# Make sure TextFinder.desktop exists in your source directory
install(FILES ${PROJECT_NAME}.desktop
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)

# Mandatory for linuxdeploy: Install the icon
# Make sure icon.png exists in your source directory
install(FILES icon.png
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps)
# qt_generate_deploy_app_script(
#     TARGET miniSerialMonitor
#     OUTPUT_SCRIPT deploy_script
#     NO_UNSUPPORTED_PLATFORM_ERROR
# )
# install(SCRIPT ${deploy_script})
qt_add_resources(miniSerialMonitor "resources"
    PREFIX "/"
    FILES
        icons/document-save.svg
        icons/view-refresh.svg
)
