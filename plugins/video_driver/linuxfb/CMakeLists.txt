
set (LINUXFB_SOURCE_FILES
    libinput_device.c
    libinput_device_manager.c
    vd_lnxfb.c
)

set (LINUXFB_MODULES
    gobject-2.0
    gmodule-export-2.0
    libudev
    libinput
    xkbcommon
)
pkg_check_modules (LINUXFB_DEPS REQUIRED ${LINUXFB_MODULES})

string (REPLACE ";" " " link_flags "${LINUXFB_DEPS_LDFLAGS}")

add_library (plugin-linuxfb MODULE ${LINUXFB_SOURCE_FILES})
target_compile_options (plugin-linuxfb PRIVATE "-Wall" "-Werror")
target_include_directories (plugin-linuxfb
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/src/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LINUXFB_DEPS_INCLUDE_DIRS}
)
target_link_libraries (plugin-linuxfb ${LINUXFB_DEPS_LIBRARIES} ${SHARED_LIBRARY_TARGET})
set_target_properties (plugin-linuxfb PROPERTIES
    OUTPUT_NAME ${PACKAGE_NAME}-vdriver-linuxfb
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
    LINK_FLAGS "${link_flags}"
)
add_dependencies (plugins plugin-linuxfb)
install (TARGETS plugin-linuxfb LIBRARY DESTINATION ${CMAKE_INSTALL_PKGPLUGINDIR})
