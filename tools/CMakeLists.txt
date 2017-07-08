
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")

find_package (PkgConfig REQUIRED)
find_package (Valac REQUIRED)

pkg_check_modules (GRX_CALIBRATE_DEPS REQUIRED
    glib-2.0
    gobject-2.0
    gio-2.0
    gudev-1.0
)

vala2c (GRX_CALIBRATE_C
    SOURCE_FILES
        calibrate-touch.vala
    PACKAGES
        grx-3.0
        gio-2.0
        gudev-1.0
    VAPI_DIRS
        ${CMAKE_SOURCE_DIR}/bindings/vala/
    DEPENDS
        ${CMAKE_SOURCE_DIR}/bindings/vala/grx-3.0.vapi
)

add_executable (grx-calibrate-touch ${GRX_CALIBRATE_C})
target_compile_options (grx-calibrate-touch PRIVATE "-w")
target_include_directories (grx-calibrate-touch PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/src/include
    ${GRX_CALIBRATE_DEPS_INCLUDE_DIRS}
)
target_link_libraries (grx-calibrate-touch ${GRX_CALIBRATE_DEPS_LIBRARIES} ${SHARED_LIBRARY_TARGET})
if (CMAKE_CROSSCOMPILING)
    # not sure why we need to do this when cross compiling
    target_link_libraries (grx-calibrate-touch "-pthread" "-lresolv")
endif (CMAKE_CROSSCOMPILING)

install (TARGETS grx-calibrate-touch RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

endif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
