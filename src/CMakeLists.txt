include_directories (include)

set (LINUX_SOURCE_FILES
    application/linux_console.c
    draw/bitblt.c
    draw/bitbltnc.c
    draw/bitblt1b.c
    draw/box.c
    draw/boxnc.c
    draw/clearclp.c
    draw/clearctx.c
    draw/clearscr.c
    draw/drwinlne.c
    draw/fillbox.c
    draw/fillboxn.c
    draw/frambox.c
    draw/framboxn.c
    draw/getscl.c
    draw/line.c
    draw/linenc.c
    draw/majorln1.c
    draw/majorln2.c
    draw/majorln3.c
    draw/majorln4.c
    draw/pixel.c
    draw/pixelc.c
    draw/plot.c
    draw/putscl.c
    draw/flodspil.c
    fdrivers/dotab8.c
    fdrivers/ftable.c
    fdrivers/genblit.c
    fdrivers/gengiscl.c
    fdrivers/genptscl.c
    fdrivers/genstrch.c
    fdrivers/pblitr2r.c
    fdrivers/pblitr2v.c
    fdrivers/pblitv2r.c
    fdrivers/pblitv2v.c
    fdrivers/ram1.c
    fdrivers/ram4.c
    fdrivers/ram8.c
    fdrivers/ram16.c
    fdrivers/ram24.c
    fdrivers/ram32l.c
    fdrivers/ram32h.c
    fdrivers/rblit_14.c
    fonts/fdv_grx.c
    fonts/fdv_raw.c
    fonts/fdv_fna.c
    fonts/fdv_win.c
    fonts/fdtable.c
    fonts/pc6x8.c
    fonts/pc8x8.c
    fonts/pc8x14.c
    fonts/pc8x16.c
    image/ialloc.c
    image/ibuild.c
    image/ifbox.c
    image/ihline.c
    image/iinverse.c
    image/imginlne.c
    image/iplot.c
    image/istretch.c
    gformats/ctx2pnm.c
    gformats/pnm2ctx.c
    mouse/bldcurs.c
    mouse/drawcurs.c
    mouse/lnxinpu2.c
    mouse/lnxkeys.c
    mouse/mouinfo.c
    mouse/mouinlne.c
    mouse/mscursor.c
    pattern/fillpatt.c
    pattern/makepat.c
    pattern/patfbits.c
    pattern/patfbox.c
    pattern/patfcvxp.c
    pattern/patfline.c
    pattern/patfplot.c
    pattern/patfpoly.c
    pattern/patternf.c
    pattern/pattfldf.c
    pattern/pattline.c
    pattern/pattpoly.c
    pattern/pfcirc.c
    pattern/pfcirca.c
    pattern/pfelli.c
    pattern/pfellia.c
    pattern/ptcirc.c
    pattern/ptcirca.c
    pattern/ptelli.c
    pattern/ptellia.c
    setup/clip.c
    setup/clrinfo.c
    setup/clrinlne.c
    setup/colorbw.c
    setup/colorega.c
    setup/colors.c
    setup/context.c
    setup/cxtinfo.c
    setup/cxtinlne.c
    setup/drvinfo.c
    setup/drvinlne.c
    setup/fframe.c
    setup/fgeom.c
    setup/hooks.c
    setup/modewalk.c
    setup/setdrvr.c
    setup/setmode.c
    setup/viewport.c
    shape/circle1.c
    shape/circle2.c
    shape/circle3.c
    shape/circle4.c
    shape/drawpoly.c
    shape/fillcir1.c
    shape/fillcir2.c
    shape/fillcnvx.c
    shape/fillell1.c
    shape/fillell2.c
    shape/fillpoly.c
    shape/flood.c
    shape/floodfil.c
    shape/genellip.c
    shape/polygon.c
    shape/polyline.c
    shape/scancnvx.c
    shape/scanellp.c
    shape/scanpoly.c
    shape/solidfil.c
    text/buildaux.c
    text/buildfnt.c
    text/convfont.c
    text/drawstrg.c
    text/drawtext.c
    text/drwstrg.c
    text/dumpfna.c
    text/dumpfont.c
    text/dumptext.c
    text/epatstrg.c
    text/fntinlne.c
    text/fontinfo.c
    text/fontpath.c
    text/loadfont.c
    text/pattstrg.c
    text/propwdt.c
    text/type.c
    text/unloadfn.c
    user/ubox.c
    user/ucbox.c
    user/uccirc.c
    user/uccirca.c
    user/ucelli.c
    user/ucellia.c
    user/ucirc.c
    user/ucirca.c
    user/ucircf.c
    user/ucircfa.c
    user/ucline.c
    user/ucpolyg.c
    user/ucpolyl.c
    user/udrwchar.c
    user/udrwstrg.c
    user/uelli.c
    user/uellia.c
    user/uellif.c
    user/uellifa.c
    user/ufcpolyg.c
    user/ufillbox.c
    user/uflood.c
    user/ufpolyg.c
    user/uframbox.c
    user/ugetwin.c
    user/uhline.c
    user/uline.c
    user/upbox.c
    user/upcirc.c
    user/upcirca.c
    user/upelli.c
    user/upellia.c
    user/upfbox.c
    user/upfcirc.c
    user/upfcirca.c
    user/upfcpoly.c
    user/upfelli.c
    user/upfellia.c
    user/upfflood.c
    user/upfline.c
    user/upfplot.c
    user/upfpolyg.c
    user/upixel.c
    user/upixelc.c
    user/upline.c
    user/uplot.c
    user/upolygon.c
    user/upolylin.c
    user/uppolyg.c
    user/uppolyl.c
    user/usercord.c
    user/usetwin.c
    user/utextxy.c
    user/uvline.c
    utils/resize.c
    utils/ordswap.c
    utils/shiftscl.c
    utils/tmpbuff.c
    vdrivers/vd_lnxfb.c
    vdrivers/vd_mem.c
    vdrivers/vtable.c
    wideline/ccirc.c
    wideline/ccirca.c
    wideline/celli.c
    wideline/cellia.c
    wideline/custbox.c
    wideline/custline.c
    wideline/custplne.c
    wideline/custpoly.c
    wideline/drwcpoly.c
    gformats/ctx2jpg.c
    gformats/jpg2ctx.c
    gformats/ctx2png.c
    gformats/png2ctx.c
)

if (GRX_DEBUG)
    list (APPEND LINUX_SOURCE_FILES utils/dbgprint.c)
endif (GRX_DEBUG)

add_library (${LIBRARY_NAME} OBJECT ${LINUX_SOURCE_FILES})
set_target_properties (${LIBRARY_NAME} PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_compile_definitions (${LIBRARY_NAME} PUBLIC
    "GRX_DEFAULT_FONT_PATH=\"${CMAKE_INSTALL_PKGDATADIR}/fonts\"")

add_library (${SHARED_LIBRARY_NAME} SHARED $<TARGET_OBJECTS:${LIBRARY_NAME}>)
target_link_libraries (${SHARED_LIBRARY_NAME} ${DEPS_LIBRARIES} ${JPEG_LIBRARIES})
set_target_properties (${SHARED_LIBRARY_NAME} PROPERTIES
    VERSION "${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
    SOVERSION "${PROJECT_VERSION_MINOR}"
    OUTPUT_NAME ${LIBRARY_NAME}
)
install (TARGETS ${SHARED_LIBRARY_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_library (${STATIC_LIBRARY_NAME} STATIC $<TARGET_OBJECTS:${LIBRARY_NAME}>)
target_link_libraries (${STATIC_LIBRARY_NAME} ${DEPS_LIBRARIES} ${JPEG_LIBRARIES})
set_target_properties (${STATIC_LIBRARY_NAME} PROPERTIES
    OUTPUT_NAME ${LIBRARY_NAME}
)
install (TARGETS ${STATIC_LIBRARY_NAME} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

get_property (INCLUDE_DIRS DIRECTORY .. PROPERTY INCLUDE_DIRECTORIES)

# Generating .gir and .typelib does not work when cross compiling because it
# uses host architecture.
if (NOT CMAKE_CROSSCOMPILING)

set (GTKDOC_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/include/grx/color.h
    ${CMAKE_SOURCE_DIR}/include/grx/common.h
    ${CMAKE_SOURCE_DIR}/include/grx/context.h
    ${CMAKE_SOURCE_DIR}/include/grx/draw.h
    ${CMAKE_SOURCE_DIR}/include/grx/draw_nc.h
    ${CMAKE_SOURCE_DIR}/include/grx/gformats.h
    ${CMAKE_SOURCE_DIR}/include/grx/image.h
    ${CMAKE_SOURCE_DIR}/include/grx/linux_console_application.h
    ${CMAKE_SOURCE_DIR}/include/grx/mode.h
    ${CMAKE_SOURCE_DIR}/include/grx/pattern.h
    ${CMAKE_SOURCE_DIR}/include/grx/text.h
    ${CMAKE_SOURCE_DIR}/include/grx/user.h
    ${CMAKE_SOURCE_DIR}/include/grx/wideline.h
    application/linux_console.c
    draw/bitblt.c
    draw/bitbltnc.c
    draw/bitblt1b.c
    draw/box.c
    draw/boxnc.c
    draw/clearclp.c
    draw/clearctx.c
    draw/clearscr.c
    draw/drwinlne.c
    draw/fillbox.c
    draw/fillboxn.c
    draw/frambox.c
    draw/framboxn.c
    draw/getscl.c
    draw/line.c
    draw/linenc.c
    draw/majorln1.c
    draw/majorln2.c
    draw/majorln3.c
    draw/majorln4.c
    draw/pixel.c
    draw/pixelc.c
    draw/plot.c
    draw/putscl.c
    draw/flodspil.c
    image/ibuild.c
    image/iinverse.c
    image/imginlne.c
    image/istretch.c
    pattern/makepat.c
    setup/context.c
    setup/fgeom.c
    setup/hooks.c
    shape/circle1.c
    shape/circle2.c
    shape/circle3.c
    shape/circle4.c
    shape/drawpoly.c
    shape/fillcir1.c
    shape/fillcir2.c
    shape/fillcnvx.c
    shape/fillell1.c
    shape/fillell2.c
    shape/fillpoly.c
    shape/flood.c
    shape/floodfil.c
    shape/genellip.c
    shape/polygon.c
    shape/polyline.c
    shape/scancnvx.c
    shape/scanellp.c
    shape/scanpoly.c
    shape/solidfil.c
    text/type.c
)

find_package (GirScanner REQUIRED)
include (GirScanner)
add_gir (GIR ${SHARED_LIBRARY_NAME} ${GIR_NAME} ${GIR_VERSION}
    INCLUDES
        GLib-2.0
        Gio-2.0
    INCLUDE_DIRS
        ${INCLUDE_DIRS}
    ARGS
        --warn-all
    FILES
        ${GTKDOC_SOURCE_FILES}
)
install (FILES $<TARGET_PROPERTY:${GIR},GIR_FILE_NAME>
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/gir-1.0/)

find_package (GirCompiler)
include (GirCompiler)
add_typelib (TYPELIB ${GIR})
install (FILES $<TARGET_PROPERTY:${TYPELIB},TYPELIB_FILE_NAME>
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/girepository-1.0/)

endif (NOT CMAKE_CROSSCOMPILING)

find_package (GtkDoc REQUIRED)

file (GLOB INTERNAL_HEADERS */*.h */*/*.h)

configure_file (${CMAKE_SOURCE_DIR}/doc/reference/grx-docs.xml.in
    grx-docs.xml @ONLY)

gtk_doc_add_module (grx ${CMAKE_SOURCE_DIR}/src
    XML
        ${CMAKE_CURRENT_BINARY_DIR}/grx-docs.xml
    SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include/grx
    SUFFIXES
        "h"
        "c"
    IGNOREHEADERS
        ${INTERNAL_HEADERS}
    IGNOREFILES
        ${INTERNAL_HEADERS}
    LIBRARIES
        ${SHARED_LIBRARY_NAME}
    DEPENDS
        ${GTKDOC_SOURCE_FILES}
)

add_custom_target (docs ALL DEPENDS doc-grx)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/grx/html
        DESTINATION ${CMAKE_INSTALL_DOCDIR})
