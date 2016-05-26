
set (GRX_GSCHEMA org.ev3dev.grx.gschema.xml)
configure_file (org.ev3dev.grx.gschema.xml ${GRX_GSCHEMA} COPYONLY)

set (GSCHEMA_COMPILED gschemas.compiled)
add_custom_command (OUTPUT ${GSCHEMA_COMPILED}
    COMMAND
        glib-compile-schemas
    ARGS
        ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/${GRX_GSCHEMA}
)
add_custom_target (gschema ALL
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/${GSCHEMA_COMPILED}
)
