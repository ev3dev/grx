
find_package (GirDocTool REQUIRED)

macro (add_yelp_doc LANGUAGE)
    add_gir_doc (doc-yelp-${LANGUAGE} ${LANGUAGE}
        GIR_TARGET ${GIR_TARGET}
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${LANGUAGE})

    add_dependencies(doc doc-yelp-${LANGUAGE})

    install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${LANGUAGE} DESTINATION ${CMAKE_INSTALL_DOCDIR}/yelp)
endmacro ()

add_yelp_doc (gjs)
add_yelp_doc (python)
