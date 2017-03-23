
find_program (PANDOC pandoc)

macro (add_man_page MAN_PAGE)
    set (one_value_args SOURCE SECTION HEADER)
    cmake_parse_arguments (MAN "" "${one_value_args}" "" "${ARGN}")

    add_custom_command (OUTPUT "${MAN_PAGE}"
        COMMAND "${PANDOC}"
            --output="${CMAKE_CURRENT_BINARY_DIR}/${MAN_PAGE}"
            --standalone
            --variable=section:"${MAN_SECTION}"
            --variable=header:"${MAN_HEADER}"
            --variable=footer:"${PROJECT_VERSION}"
            --to=man
            "${CMAKE_CURRENT_SOURCE_DIR}/${MAN_SOURCE}"
        DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/${MAN_SOURCE}"
    )
    list (APPEND MAN_PAGES "${CMAKE_CURRENT_BINARY_DIR}/${MAN_PAGE}")
endmacro ()

add_man_page (grx-calibrate-touch.1
    SOURCE grx-calibrate-touch.rst
    SECTION 1
    HEADER "User's Manual"
)

install (FILES ${MAN_PAGES} DESTINATION ${CMAKE_INSTALL_DOCDIR}/man)

add_custom_target (man-pages DEPENDS ${MAN_PAGES})
add_dependencies (doc man-pages)
