set (LIBRARY_NAME ${PROJECT_NAME}-${GIR_VERSION})

include_directories (include)

set (LINUX_SOURCE_FILES
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

add_library (${LIBRARY_NAME} OBJECT ${LINUX_SOURCE_FILES})
set_target_properties (${LIBRARY_NAME} PROPERTIES POSITION_INDEPENDENT_CODE 1)
target_compile_definitions (${LIBRARY_NAME} PUBLIC
    "GRX_DEFAULT_FONT_PATH=\"${CMAKE_INSTALL_PKGDATADIR}/fonts\"")

add_library (${LIBRARY_NAME}-shared SHARED $<TARGET_OBJECTS:${LIBRARY_NAME}>)
target_link_libraries (${LIBRARY_NAME}-shared ${DEPS_LIBRARIES} ${JPEG_LIBRARIES})
set_target_properties (${LIBRARY_NAME}-shared PROPERTIES
    VERSION "${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
    SOVERSION "${PROJECT_VERSION_MINOR}"
    OUTPUT_NAME ${LIBRARY_NAME}
)
install (TARGETS ${LIBRARY_NAME}-shared LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_library (${LIBRARY_NAME}-static STATIC $<TARGET_OBJECTS:${LIBRARY_NAME}>)
target_link_libraries (${LIBRARY_NAME}-static ${DEPS_LIBRARIES} ${JPEG_LIBRARIES})
set_target_properties (${LIBRARY_NAME}-static PROPERTIES
    OUTPUT_NAME ${LIBRARY_NAME}
)
install (TARGETS ${LIBRARY_NAME}-static ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

get_property (INCLUDE_DIRS DIRECTORY .. PROPERTY INCLUDE_DIRECTORIES)

find_package (GirScanner REQUIRED)
include (GirScanner)
add_gir (GIR ${LIBRARY_NAME}-shared ${GIR_NAME} ${GIR_VERSION}
    INCLUDES
        GLib-2.0
    INCLUDE_DIRS
        ${INCLUDE_DIRS}
    FILES
        ${CMAKE_SOURCE_DIR}/include/grx-3.0.h
        image/ibuild.c
        image/iinverse.c
        image/imginlne.c
        image/istretch.c
        pattern/makepat.c
        setup/context.c
        setup/fgeom.c
        text/type.c
)
install (FILES $<TARGET_PROPERTY:${GIR},GIR_FILE_NAME>
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/gir-1.0/)

find_package (GirCompiler)
include (GirCompiler)
add_typelib (TYPELIB ${LIBRARY_NAME}-shared ${GIR})
install (FILES $<TARGET_PROPERTY:${TYPELIB},TYPELIB_FILE_NAME>
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/girepository-1.0/)
