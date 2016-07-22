
include (ValaPrecompile)
find_package(PkgConfig REQUIRED)

pkg_check_modules (EXAMPLE_DEPS REQUIRED glib-2.0 gio-2.0 gobject-2.0)
add_definitions (${EXAMPLE_DEPS})

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
target_include_directories (vala-life PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/src/include
    ${EXAMPLE_DEPS_INCLUDE_DIRS}
)
target_link_libraries (vala-life ${EXAMPLE_DEPS_LIBRARIES} ${SHARED_LIBRARY_NAME})
if (CMAKE_CROSSCOMPILING)
    # Cross compiling misses a few automatic dependencies for some reason
    target_link_libraries(vala-life "-pthread" "-lresolv")
endif (CMAKE_CROSSCOMPILING)
