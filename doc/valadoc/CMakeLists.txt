
find_package (ValaDoc REQUIRED)

valadoc (doc-vala
    PACKAGES
        glib-2.0
        gio-2.0
    PACKAGE_NAME
        ${PACKAGE_NAME}
    PACKAGE_VERSION
        ${PACKAGE_VERSION}
    SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/bindings/vala/grx-3.0.vapi
)
add_dependencies (doc doc-vala)
install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/valadoc DESTINATION ${CMAKE_INSTALL_DOCDIR}/vala)
