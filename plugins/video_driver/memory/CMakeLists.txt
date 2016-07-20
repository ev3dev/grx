
set (MEMORY_SOURCE_FILES
    vd_mem.c
)

set (MEMORY_MODULES
    gmodule-export-2.0
)
pkg_check_modules (MEMORY_DEPS REQUIRED ${MEMORY_MODULES})

add_library (grx_memory SHARED ${MEMORY_SOURCE_FILES})
target_compile_options (grx_memory PUBLIC "-Wall" "-Werror")
target_include_directories (grx_memory
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/src/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${MEMORY_DEPS_INCLUDE_DIRS}
)
target_link_libraries (grx_memory ${MEMORY_DEPS_LIBRARIES} ${SHARED_LIBRARY_NAME})
set_target_properties (grx_memory PROPERTIES
    VERSION "${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
    SOVERSION "${PROJECT_VERSION_MINOR}"
    OUTPUT_NAME ${LIBRARY_NAME}-plugin-memory
)
install (TARGETS grx_memory LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
