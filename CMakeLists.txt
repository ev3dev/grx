cmake_minimum_required (VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

project ("grx" VERSION 3.1.1 LANGUAGES C)
set (ABI_VERSION 2)
set (GIR_NAME "Grx")
set (GIR_VERSION "3.0")
set (PACKAGE_NAME ${PROJECT_NAME}-${GIR_VERSION})

# Configuration settings

set (GRX_DEBUG "OFF" CACHE STRING "Debug message flags")
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    set (GRX_PLUGIN_LINUXFB "Yes" CACHE BOOL "Enable Linux framebuffer video driver plugin")
else ()
    set (GRX_PLUGIN_LINUXFB "No")
endif ()
set (GRX_PLUGIN_GTK3 "Yes" CACHE BOOL "Enable GTK+ 3 video driver plugin")
set (GRX_ENABLE_DOC "Yes" CACHE BOOL "Enable building docs")

# we have local cmake modules

list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# macOS/brew stuff
if (APPLE)
    # see `brew info gettext` caveats
    link_directories (/usr/local/opt/gettext/lib)
    include_directories (/usr/local/opt/gettext/include)
    # .typelib does not work without this
    set (CMAKE_MACOSX_RPATH No)
endif (APPLE)

# global stuff

find_package (PkgConfig REQUIRED)
include(GNUInstallDirs)
set (CMAKE_INSTALL_PKGDATADIR ${CMAKE_INSTALL_DATAROOTDIR}/${PACKAGE_NAME})
set (CMAKE_INSTALL_PKGLIBDIR ${CMAKE_INSTALL_LIBDIR}/${PACKAGE_NAME})
set (CMAKE_INSTALL_PKGPLUGINDIR ${CMAKE_INSTALL_PKGLIBDIR}/plugins/${ABI_VERSION})

set (SHARED_LIBRARY_TARGET ${PACKAGE_NAME}-shared)
set (STATIC_LIBRARY_TARGET ${PACKAGE_NAME}-static)
set (GIR_TARGET ${PACKAGE_NAME}-gir)
set (TYPELIB_TARGET ${PACKAGE_NAME}-typelib)

# subdirectories

# src goes first for depedency reasons
add_subdirectory (src)

add_subdirectory (doc)
add_subdirectory (plugins)
add_subdirectory (tools)
add_subdirectory (example)
add_subdirectory (test EXCLUDE_FROM_ALL)

# these should probably be move do subdirectories too

install (FILES bindings/vala/grx-3.0.vapi
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/vala/vapi/)

install (FILES include/grx-3.0.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (DIRECTORY include/grx DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
