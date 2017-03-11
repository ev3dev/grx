
set (PROGRAMS
    input
    life
)


foreach (PROG ${PROGRAMS})
    add_custom_target (run-python-${PROG}
        ${CMAKE_COMMAND} -E env
            LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src
            GI_TYPELIB_PATH=${CMAKE_BINARY_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/${PROG}.py
        DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/${PROG}.py
            ${SHARED_LIBRARY_NAME}
            ${TYPELIB}
    )
endforeach (PROG ${PROGRAMS})
