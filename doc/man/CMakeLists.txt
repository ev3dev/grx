
find_package (Pandoc)

add_man_page (grx-calibrate-touch.1
    SECTION 1
    HEADER "User's Manual"
    FOOTER "${PROJECT_VERSION}"
    grx-calibrate-touch.rst
)
list (APPEND MAN_PAGES grx-calibrate-touch.1)

install (FILES ${MAN_PAGES} DESTINATION ${CMAKE_INSTALL_DOCDIR}/man)

add_custom_target (doc-man-pages DEPENDS ${MAN_PAGES})
add_dependencies (doc doc-man-pages)
