
set (GTK3_SOURCE_FILES
    gtk3_device.c
    gtk3_device_manager.c
    vd_gtk3.c
)

set (GTK3_MODULES
    gmodule-export-2.0
    gtk+-3.0
)
pkg_check_modules (GTK3_DEPS REQUIRED ${GTK3_MODULES})

add_library (grx_gtk3 SHARED ${GTK3_SOURCE_FILES})
target_compile_options (grx_gtk3 PUBLIC "-Wall" "-Werror")
target_include_directories (grx_gtk3
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/src/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${GTK3_DEPS_INCLUDE_DIRS}
)
target_link_libraries (grx_gtk3 ${GTK3_DEPS_LIBRARIES} ${SHARED_LIBRARY_NAME})
set_target_properties (grx_gtk3 PROPERTIES
    OUTPUT_NAME ${LIBRARY_NAME}-vdriver-gtk3
)
install (TARGETS grx_gtk3 LIBRARY DESTINATION ${CMAKE_INSTALL_PKGPLUGINDIR})
