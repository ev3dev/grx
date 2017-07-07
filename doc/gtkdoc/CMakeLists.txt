
find_package (GtkDoc REQUIRED)

file (GLOB internalHeaders
    ${CMAKE_SOURCE_DIR}/src/*/*.h
    ${CMAKE_SOURCE_DIR}/src/*/*/*.h
)

configure_file (grx-docs.xml.in grx-docs.xml @ONLY)

gtk_doc_add_module (grx
    XML
        ${CMAKE_CURRENT_BINARY_DIR}/grx-docs.xml
    SOURCE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/include/grx
        ${CMAKE_BINARY_DIR}/src/include/grx
    SUFFIXES
        "h"
        "c"
    IGNOREHEADERS
        ${internalHeaders}
    IGNOREFILES
        ${internalHeaders}
    LIBRARIES
        ${SHARED_LIBRARY_TARGET}
)

add_dependencies (doc doc-grx)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/grx/html
        DESTINATION ${CMAKE_INSTALL_DOCDIR})
