
set (PROGRAMS
    fonts.py
    input.py
    intl.py
    life.py
    lines.py
    pixels.py
)

foreach (FILE_NAME ${PROGRAMS})
    get_filename_component(TARGET_SUFFIX ${FILE_NAME} NAME_WE)
    add_custom_target (run-python-${TARGET_SUFFIX}
        ${CMAKE_COMMAND} -E env
            LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/src
            GI_TYPELIB_PATH=${CMAKE_BINARY_DIR}/src
            ${CMAKE_CURRENT_SOURCE_DIR}/${FILE_NAME}
        DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/${FILE_NAME}
            ${SHARED_LIBRARY_NAME}
            ${TYPELIB_TARGET}
    )
    install(PROGRAMS ${FILE_NAME} DESTINATION ${CMAKE_INSTALL_DOCDIR}/examples/python)
endforeach (FILE_NAME ${PROGRAMS})
