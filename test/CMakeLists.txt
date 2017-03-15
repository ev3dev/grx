find_package (PkgConfig)
pkg_check_modules (TEST_DEPS REQUIRED
    glib-2.0
    gobject-2.0
    gio-2.0
)

add_executable (demogrx demogrx.c gfaz.c)
target_include_directories (demogrx PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/src/include
    ${TEST_DEPS_INCLUDE_DIRS}
)
target_link_libraries (demogrx
    ${LIBRARY_NAME}-shared
    m
    ${TEST_DEPS_LIBRARIES}
)

set (PROGRAMS
    arctest
    bb1test
    blittest
    circtest
    cliptest
    colorops
    curstest
    imgtest
    jpgtest
    keys
    life
    linetest
    memtest
    mousetst
    pcirctst
    pnmtest
    pngtest
    polytest
    rgbtest
    sbctest
    scroltst
    speedtst
    winclip
    wintest
)

foreach (PROG ${PROGRAMS})
    add_executable (${PROG} ${PROG}.c)
    target_include_directories (${PROG} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/src/include
        ${TEST_DEPS_INCLUDE_DIRS}
    )
    target_link_libraries (${PROG}
        ${SHARED_LIBRARY_NAME}
        m
        ${TEST_DEPS_LIBRARIES}
    )
    add_dependencies (demogrx ${PROG})
endforeach ()
