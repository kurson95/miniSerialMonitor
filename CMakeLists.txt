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

# Add this at the end of your CMakeLists.txt file

# Specify the generator for Debian packages

# Set basic package information
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Kubara <kurson95@proton.me>")
set(CPACK_PACKAGE_NAME "miniSerialMonitor")
set(CPACK_PACKAGE_VERSION "0.3")

if(EXISTS "/etc/os-release")
    file(READ "/etc/os-release" OS_RELEASE_CONTENT)

    # Check if the environment is Debian or Ubuntu based
    if(OS_RELEASE_CONTENT MATCHES "ID=ubuntu" OR OS_RELEASE_CONTENT MATCHES "ID=debian" OR OS_RELEASE_CONTENT MATCHES "ID_LIKE=.*debian.*")
        message(STATUS "Detected Debian/Ubuntu system. Configuring CPack for .deb package.")

        set(CPACK_GENERATOR "DEB")
        set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON) # Auto-detect shared library dependencies

    # Check if the environment is Fedora or RHEL based
    elseif(OS_RELEASE_CONTENT MATCHES "ID=fedora" OR OS_RELEASE_CONTENT MATCHES "ID_LIKE=.*fedora.*" OR OS_RELEASE_CONTENT MATCHES "ID_LIKE=.*rhel.*")
        message(STATUS "Detected Fedora/RHEL system. Configuring CPack for .rpm package.")

        set(CPACK_GENERATOR "RPM")
        set(CPACK_RPM_PACKAGE_LICENSE "GPLv3") # RPM strictly requires a license field

    else()
        # Fallback for other distributions (e.g., Arch Linux, openSUSE)
        message(WARNING "Unknown distribution. Falling back to .tar.gz archive.")
        set(CPACK_GENERATOR "TGZ")
    endif()

else()
    message(WARNING "/etc/os-release not found. Falling back to .tar.gz archive.")
    set(CPACK_GENERATOR "TGZ")
endif()

# Finalize CPack configuration
include(CPack)