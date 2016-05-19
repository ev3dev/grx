cmake_minimum_required (VERSION 3.0)
cmake_policy(SET CMP0048 NEW)

project ("grx" VERSION 3.0.0 LANGUAGES C)
set (GIR_NAME "Grx")
set (GIR_VERSION "3.0")
set (PACKAGE_NAME ${PROJECT_NAME}-${GIR_VERSION})

# Configuration settings

set (GRX_DEBUG "OFF" CACHE STRING "Debug message flags")
set (GRX_LINUXFB "Yes" CACHE BOOL "Enable Linux framebuffer driver")

if (GRX_DEBUG)
    add_definitions ("-DDEBUG=${GRX_DEBUG}")
endif (GRX_DEBUG)

if (GRX_LINUXFB)
    add_definitions ("-DFRAMEBUFFER_DRIVER -DLFB_BY_NEAR_POINTER")
endif (GRX_LINUXFB)

list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(GNUInstallDirs)
set (CMAKE_INSTALL_PKGDATADIR ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})

find_package (JPEG REQUIRED)
include_directories (${JPEG_INCLUDE_DIR})

set (MODULES libpng12 glib-2.0 gio-2.0 gobject-2.0)
find_package (PkgConfig REQUIRED)
pkg_check_modules (DEPS REQUIRED ${MODULES})
include_directories (${DEPS_INCLUDE_DIRS})

include_directories (include)

set (LIBRARY_NAME ${PROJECT_NAME}-${GIR_VERSION})
set (SHARED_LIBRARY_NAME ${LIBRARY_NAME}-shared)
set (STATIC_LIBRARY_NAME ${LIBRARY_NAME}-static)
add_subdirectory (src)
add_subdirectory (example EXCLUDE_FROM_ALL)
add_subdirectory (test EXCLUDE_FROM_ALL)

# convert CMake list to space separated string for pkgconfig file
string (REPLACE ";" " " PKGCONFIG_DEPS "${MODULES}")
configure_file (grx.pc.in ${PACKAGE_NAME}.pc @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig/)

install (FILES bindings/vala/grx-3.0.vapi
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/vala/vapi/)

file (GLOB FONT_FILES fonts/*)
install (FILES ${FONT_FILES} DESTINATION ${CMAKE_INSTALL_PKGDATADIR}/fonts/)

install (FILES include/grx-3.0.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
