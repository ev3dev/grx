
add_executable (demogrx demogrx.c gfaz.c)
target_link_libraries (demogrx ${LIBRARY_NAME}-shared m)

set (PROGRAMS
    arctest
    bb1test
    blittest
    circtest
    cliptest
    colorops
    curstest
    fontdemo
    fonttest
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
    textpatt
    winclip
    wintest
)

foreach (PROG ${PROGRAMS})
    add_executable (${PROG} ${PROG}.c)
    target_link_libraries (${PROG} ${SHARED_LIBRARY_NAME} m)
    add_dependencies (demogrx ${PROG})
endforeach ()
