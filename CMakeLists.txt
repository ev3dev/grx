cmake_minimum_required (VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

project ("grx" VERSION 3.0.0 LANGUAGES C)
set (GIR_NAME "Grx")
set (GIR_VERSION "3.0")
set (PACKAGE_NAME ${PROJECT_NAME}-${GIR_VERSION})

# Configuration settings

set (GRX_DEBUG "OFF" CACHE STRING "Debug message flags")
set (GRX_LINUXFB "Yes" CACHE BOOL "Enable Linux framebuffer video driver")
set (GRX_GTK3 "Yes" CACHE BOOL "Enable GTK+ 3 video driver")

# we have local cmake modules

list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# global stuff

find_package (PkgConfig REQUIRED)
include(GNUInstallDirs)
set (CMAKE_INSTALL_PKGDATADIR ${CMAKE_INSTALL_DATAROOTDIR}/${PACKAGE_NAME})

set (LIBRARY_NAME ${PROJECT_NAME}-${GIR_VERSION})
set (SHARED_LIBRARY_NAME ${LIBRARY_NAME}-shared)
set (STATIC_LIBRARY_NAME ${LIBRARY_NAME}-static)

# subdirectories

add_subdirectory (plugins)
add_subdirectory (src)
add_subdirectory (tools)
add_subdirectory (example EXCLUDE_FROM_ALL)
add_subdirectory (test EXCLUDE_FROM_ALL)

# these should probably be move do subdirectories too

install (FILES bindings/vala/grx-3.0.vapi
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/vala/vapi/)

file (GLOB FONT_FILES fonts/*)
install (FILES ${FONT_FILES} DESTINATION ${CMAKE_INSTALL_PKGDATADIR}/fonts/)

install (FILES include/grx-3.0.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (DIRECTORY include/grx DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
