
find_package (PkgConfig REQUIRED)
find_package (Valac REQUIRED)

pkg_check_modules (EXAMPLE_DEPS REQUIRED glib-2.0 gio-2.0 gobject-2.0)

macro (add_demo name)
    # optionally include demo.vala
    if ("${ARGV1}" STREQUAL "SIMPLE")
        set (simpleSource "demo.vala")
    else ()
        unset (simpleSource)
    endif ()

    vala2c (vala-${name}-vala
        SOURCE_FILES
            ${name}.vala
            ${simpleSource}
        PACKAGES
            grx-3.0
            gio-2.0
            posix
        VAPI_DIRS
            ${CMAKE_SOURCE_DIR}/bindings/vala/
        TARGET_GLIB
            2.48
        OUTPUT_DIR
            ${name}.dir
        DEPENDS
            ${CMAKE_SOURCE_DIR}/bindings/vala/grx-3.0.vapi
    )

    add_executable (vala-${name} $<TARGET_PROPERTY:vala-${name}-vala,C_FILES>)
    set_property (TARGET vala-${name} PROPERTY OUTPUT_NAME ${name})
    target_compile_options (vala-${name} PRIVATE "-w")
    target_include_directories (vala-${name} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/src/include
        ${EXAMPLE_DEPS_INCLUDE_DIRS}
    )
    target_link_libraries (vala-${name} ${EXAMPLE_DEPS_LIBRARIES} ${SHARED_LIBRARY_TARGET})
    if (CMAKE_CROSSCOMPILING)
        # Cross compiling misses a few automatic dependencies for some reason
        target_link_libraries(vala-${name} "-pthread" "-lresolv")
    endif (CMAKE_CROSSCOMPILING)

    install (TARGETS vala-${name} DESTINATION ${CMAKE_INSTALL_PKGLIBDIR}/examples/vala)

    add_custom_target (run-vala-${name}
        COMMAND ${CMAKE_COMMAND} -E env
            GRX_PLUGIN_PATH="${CMAKE_BINARY_DIR}/plugins"
            "$<TARGET_FILE:vala-${name}>"
        DEPENDS
            vala-${name}
            plugins
    )

endmacro ()

add_demo (boxes SIMPLE)
add_demo (circles SIMPLE)
add_demo (ellipses SIMPLE)
add_demo (life)
add_demo (lines SIMPLE)
add_demo (pixels SIMPLE)
