
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

add_library (grx_linuxfb SHARED ${LINUXFB_SOURCE_FILES})
target_compile_options (grx_linuxfb PUBLIC "-Wall" "-Werror")
target_include_directories (grx_linuxfb
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/src/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LINUXFB_DEPS_INCLUDE_DIRS}
)
target_link_libraries (grx_linuxfb ${LINUXFB_DEPS_LIBRARIES} ${SHARED_LIBRARY_NAME})
set_target_properties (grx_linuxfb PROPERTIES
    OUTPUT_NAME ${LIBRARY_NAME}-plugin-linuxfb
)
install (TARGETS grx_linuxfb LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
