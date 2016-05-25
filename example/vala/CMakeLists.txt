
include (ValaPrecompile)
find_package(PkgConfig REQUIRED)

pkg_check_modules (EXAMPLE_DEPS REQUIRED glib-2.0 gio-2.0 gobject-2.0)
add_definitions (${EXAMPLE_DEPS})

vala_precompile (CALIBRATE_C CALIBRATE_VALA calibrate.vala
    PACKAGES
        grx-3.0
        gio-2.0
    OPTIONS
        --vapidir=${CMAKE_SOURCE_DIR}/bindings/vala/
    DEPENDS
        ${CMAKE_SOURCE_DIR}/bindings/vala/grx-3.0.vapi
)

add_executable (vala-calibrate ${CALIBRATE_C})
set_property (TARGET vala-calibrate PROPERTY OUTPUT_NAME calibrate)
target_link_libraries (vala-calibrate ${EXAMPLE_DEPS_LIBRARIES} ${SHARED_LIBRARY_NAME})
target_compile_options (vala-calibrate PUBLIC "-w")


vala_precompile (LIFE_C LIFE_VALA life.vala
    PACKAGES
        grx-3.0
        gio-2.0
        posix
    OPTIONS
        --vapidir=${CMAKE_SOURCE_DIR}/bindings/vala/
    DEPENDS
        ${CMAKE_SOURCE_DIR}/bindings/vala/grx-3.0.vapi
)

add_executable (vala-life ${LIFE_C})
set_property (TARGET vala-life PROPERTY OUTPUT_NAME life)
target_link_libraries (vala-life ${EXAMPLE_DEPS_LIBRARIES} ${SHARED_LIBRARY_NAME})
